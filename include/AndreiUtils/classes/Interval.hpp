//
// Created by Andrei on 20.11.23.
//

#pragma once

#include <AndreiUtils/classes/RandomNumberGenerator.hpp>
#include <AndreiUtils/traits/is_numeric.hpp>
#include <AndreiUtils/utils.hpp>
#include <stdexcept>

namespace AndreiUtils {
    template<typename T, class Enable = void>
    class Interval;

    template<class T>
    class Interval<T, typename std::enable_if<AndreiUtils::is_numeric<T>::value>::type> {
    public:
        [[nodiscard]] static Interval<T> createEmpty() {
            return Interval<T>();
        }

        [[nodiscard]] static Interval<T> createOnlyUpperBound(T maxValue) {
            return Interval<T>(std::numeric_limits<T>::min(), std::move(maxValue), true, false);
        }

        [[nodiscard]] static Interval<T> createOnlyLowerBound(T minValue) {
            return Interval<T>(std::move(minValue), std::numeric_limits<T>::max(), false, true);
        }

        [[nodiscard]] static Interval<T> createFullRange() {
            if (std::is_integral<T>::value) {
                // divide by 2 to be able to sample from the interval without constant values
                T minVal = std::numeric_limits<T>::min() / 2, maxVal = std::numeric_limits<T>::max() / 2;
                return {minVal, maxVal, true, true};
            } else if (std::is_floating_point<T>::value) {
                // divide by 2 to be able to sample from the interval without getting inf values
                T maxVal = std::numeric_limits<T>::max() / 2;
                return {-maxVal, maxVal, true, true};
            }
            return {std::numeric_limits<T>::min(), std::numeric_limits<T>::max(), true, true};
        }

        Interval() : empty(true), minValue(), maxValue(), infLower(false), infUpper(false) {}

        explicit Interval(T minValue, T maxValue) : Interval(std::move(minValue), std::move(maxValue), false, false) {}

        Interval(Interval const &other) : empty(other.empty), minValue(other.minValue), maxValue(other.maxValue),
                                          infLower(other.infLower), infUpper(other.infUpper) {}

        Interval(Interval &&other) noexcept: empty(other.empty), minValue(std::move(other.minValue)),
                                             maxValue(std::move(other.maxValue)), infLower(other.infLower),
                                             infUpper(other.infUpper) {}

        virtual ~Interval() = default;

        Interval &operator=(Interval const &other) {
            if (this != &other) {
                this->empty = other.empty;
                this->minValue = other.minValue;
                this->maxValue = other.maxValue;
                this->infLower = other.infLower;
                this->infUpper = other.infUpper;
            }
            return *this;
        }

        Interval &operator=(Interval &&other) noexcept {
            if (this != &other) {
                this->empty = other.empty;
                this->minValue = std::move(other.minValue);
                this->maxValue = std::move(other.maxValue);
                this->infLower = other.infLower;
                this->infUpper = other.infUpper;
            }
            return *this;
        }

        bool operator==(Interval const &other) noexcept {
            // intervals are the same if they're both empty or if the bounds are the same
            if (this->empty != other.empty || this->infLower != other.infLower || this->infUpper != other.infUpper) {
                return false;
            }
            if (this->empty || this->infUpper && this->infLower) {
                return true;
            }
            if (this->infUpper) {
                return AndreiUtils::equal(this->minValue, other.minValue);
            } else if (this->infLower) {
                return AndreiUtils::equal(this->maxValue, other.maxValue);
            }
            return AndreiUtils::equal(this->minValue, other.minValue) &&
                   AndreiUtils::equal(this->maxValue, other.maxValue);
        }

        bool operator!=(Interval const &other) noexcept {
            return !this->operator==(other);
        }

        [[nodiscard]] RandomNumberGenerator<T> createSampler() const {
            if (this->empty) {
                throw std::runtime_error("Can not sample from an empty interval");
            }
            return AndreiUtils::RandomNumberGenerator<T>(this->minValue, this->maxValue);
        }

        [[nodiscard]] RandomNumberGenerator<T> createSampler(std::random_device::result_type const &seed) const {
            if (this->empty) {
                throw std::runtime_error("Can not sample from an empty interval");
            }
            return AndreiUtils::RandomNumberGenerator<T>(this->minValue, this->maxValue, seed);
        }

        [[nodiscard]] Interval unify(Interval const &other) const {
            if (this->empty) {
                return other;
            } else if (other.empty) {
                return *this;
            }
            return Interval(AndreiUtils::fastMin(this->minValue, other.minValue),
                            AndreiUtils::fastMax(this->maxValue, other.maxValue));
        }

        [[nodiscard]] Interval intersect(Interval const &other) const {
            if (this->empty || other.empty || AndreiUtils::lessEqual(other.maxValue, this->minValue) ||
                AndreiUtils::lessEqual(this->maxValue, other.minValue)) {
                return Interval<T>();
            }
            return Interval(AndreiUtils::fastMax(this->minValue, other.minValue),
                            AndreiUtils::fastMin(this->maxValue, other.maxValue));
        }

        [[nodiscard]] bool isEmpty() const {
            return this->empty;
        }

        [[nodiscard]] bool hasInfLowerBound() const {
            return this->infLower;
        }

        [[nodiscard]] bool hasInfUpperBound() const {
            return this->infUpper;
        }

        // no T const & as return type because of the inf-return
        [[nodiscard]] T getMin() const {
            if (this->empty) {
                throw std::runtime_error("There is no lower bound of an empty interval!");
            }
            if (this->infLower) {
                return -std::numeric_limits<T>::infinity();
            }
            return this->minValue;
        }

        // no T const & as return type because of the inf-return
        [[nodiscard]] T getMax() const {
            if (this->empty) {
                throw std::runtime_error("There is no lower bound of an empty interval!");
            }
            if (this->infUpper) {
                return std::numeric_limits<T>::infinity();
            }
            return this->maxValue;
        }

        [[nodiscard]] T size() const {
            if (this->empty) {
                throw std::runtime_error("There is no size of an empty interval!");
            }
            if (this->infUpper || this->infLower) {
                return std::numeric_limits<T>::infinity();
            }
            return this->maxValue - this->minValue;
        }

        [[nodiscard]] std::string toString() const {
            return (this->isEmpty() ? "[]" : "[" + std::to_string(this->getMin()) + ", " +
                                             std::to_string(this->getMax()) + "]");
        }

    protected:
        Interval(T minValue, T maxValue, bool infLower, bool infUpper) :
                empty(AndreiUtils::greater<T>(this->minValue, this->maxValue)), minValue(std::move(minValue)),
                maxValue(std::move(maxValue)), infLower(infLower), infUpper(infUpper) {}

        double minValue, maxValue;
        bool empty;  // empty is initialized after min- and maxValue; thus the constructor will assign the correct value to the empty field
        bool infLower, infUpper;
    };

    using IntervalF = Interval<float>;
    using IntervalD = Interval<double>;
    using IntervalI = Interval<int>;
}