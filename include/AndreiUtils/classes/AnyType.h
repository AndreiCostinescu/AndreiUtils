//
// Created by Andrei on 07.12.22.
//

#pragma once

#include <stdexcept>
#include <string>

namespace AndreiUtils {
    /**
     * @brief This data type can be used to represent any other data type (for example, integer, floating-point,
     * single- and double-precision, user defined types, etc.).
     *
     * While the use of not explicitly declared variants such as this is not recommended, they can be of use when the
     * needed data type can only be known at runtime, when the data type is expected to vary, or when optional
     * parameters and parameter arrays are desired.
     *
     * Inspired by: https://stackoverflow.com/a/18566446
     */
    class AnyType {
    public:
        /**
         * @brief Initializes a new instance of the AnyType class.
         */
        AnyType();

        /**
         * @brief Initializes a new instance of the AnyType class.
         *
         * @param value The value to be held.
         */
        template<typename ValueType>
        explicit AnyType(ValueType const &value) : content(new Holder<ValueType>(value)) {}

        /**
         * @brief Initializes a new instance of the AnyType class.
         *
         * @param value The value to be held.
         */
        template<typename ValueType>
        explicit AnyType(ValueType &&value) : content(new Holder<ValueType>(std::forward<ValueType>(value))) {}

        /**
         * @brief Initializes a new instance of the AnyType class.
         *
         * @param other The AnyType object to copy.
         */
        AnyType(AnyType const &other);

        /**
         * @brief Initializes a new instance of the AnyType class.
         *
         * @param other The AnyType object to move.
         */
        AnyType(AnyType &&other) noexcept;

        /**
         * @brief Finalizes and instance of the Any class.
         */
        virtual ~AnyType();

        /**
         * @brief Exchange values of two objects.
         *
         * @param rhs The AnyType object to be swapped with.
         *
         * @return A reference to this.
         */
        AnyType &swap(AnyType &other);

        /**
         * @brief The copy assignment operator.
         *
         * @param other The value to be assigned.
         *
         * @return A reference to this.
         */
        template<typename ValueType>
        AnyType &operator=(ValueType const &other) {
            AnyType(other).swap(*this);
            return *this;
        }

        /**
         * @brief The move assignment operator.
         *
         * @param other The value to be assigned.
         *
         * @return A reference to this.
         */
        template<typename ValueType>
        AnyType &operator=(ValueType &&other) {
            AnyType(std::forward<ValueType>(other)).swap(*this);
            return *this;
        }

        /**
         * @brief The copy assignment operator.
         *
         * @param other The value to be assigned.
         *
         * @return A reference to this.
         */
        AnyType &operator=(AnyType const &other);

        /**
         * @brief The move assignment operator.
         *
         * @param other The value to be assigned.
         *
         * @return A reference to this.
         */
        AnyType &operator=(AnyType &&other) noexcept;

        /**
         * @brief The () operator.
         *
         * @return The held value.
         */
        template<typename ValueType>
        ValueType operator()() const {
            return this->template get<ValueType>();
        }

        /**
         * @brief Gets the underlying value.
         *
         * @return The held value.
         */
        template<typename ValueType>
        ValueType get() const {
            if (this->empty()) {
                throw std::runtime_error("AnyType is empty! Nothing to get");
            } else if (this->getType() == typeid(ValueType)) {
                return static_cast<AnyType::Holder<ValueType> *>(this->content)->held;
            } else {
                throw std::runtime_error("Wrong type to cast to (has " + this->getTypeName() + ", requested " +
                                         typeid(ValueType).name() + "!");
            }
        }

        /**
         * @brief Tells whether the holder is empty or not.
         *
         * @return <tt>true</tt> if the holder is empty; otherwise <tt>false</tt>.
         */
        [[nodiscard]] bool empty() const;

    protected:
        /**
         * @brief Gets the type of the underlying value.
         */
        [[nodiscard]] std::type_info const &getType() const;

        /**
         * @brief Gets the type name of the underlying value.
         */
        [[nodiscard]] std::string getTypeName() const;

        /**
         * @brief The place holder structure..
         */
        struct PlaceHolder {
            /**
             * @brief Finalizes and instance of the PlaceHolder class.
             */
            virtual ~PlaceHolder();

            /**
             * @brief Gets the type of the underlying value.
             */
            [[nodiscard]] virtual std::type_info const &getType() const = 0;

            /**
             * @brief Gets the type name of the underlying value.
             */
            [[nodiscard]] virtual std::string getTypeName() const = 0;

            /**
             * @brief Clones the holder.
             */
            [[nodiscard]] virtual PlaceHolder *clone() const = 0;
        };

        template<typename ValueType>
        struct Holder : public PlaceHolder {
            /**
             * @brief Initializes a new instance of the Holder class.
             *
             * @param ValueType The value to be holded.
             */
            explicit Holder(ValueType const &value) : held(value) {}

            /**
             * @brief Gets the type of the underlying value.
             */
            [[nodiscard]] std::type_info const &getType() const override {
                return typeid(ValueType);
            }

            /**
             * @brief Gets the type name of the underlying value.
             */
            [[nodiscard]] std::string getTypeName() const override {
                return typeid(ValueType).name();
            }

            /**
             * @brief Clones the holder.
             */
            [[nodiscard]] PlaceHolder *clone() const override {
                return new Holder(held);
            }

            ValueType held;
        };

        PlaceHolder *content;
    };
}
