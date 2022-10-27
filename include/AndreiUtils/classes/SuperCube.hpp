//
// Created by Andrei on 27.10.22.
//

#ifndef ANDREIUTILS_SUPERCUBE_HPP
#define ANDREIUTILS_SUPERCUBE_HPP

#include <AndreiUtils/utilsBinarySerialization.hpp>
#include <Eigen/Dense>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <type_traits>

namespace nlohmann {
    template<typename T, typename SFINAE>
    struct adl_serializer;
}

namespace AndreiUtils {
    template<int IndexDimensions>
    class SuperCubeData {
    public:
        using IndexType = Eigen::Matrix<double, IndexDimensions, 1>;

        SuperCubeData() = default;

        virtual ~SuperCubeData() = default;

        virtual IndexType getIndex() const = 0;

        virtual void saveBinary(std::ofstream &bin) const = 0;

        virtual void readBinary(std::ifstream &bin) = 0;
    };

    template<typename T, int SpatialDimensions, int SpatialDivision>
    class SuperCubeInterface {
    public:
        static_assert(SpatialDivision > 0, "SpatialDivision of the SuperCube is not positive!");
        static_assert(SpatialDimensions > 0, "SpatialDimensions of the SuperCube is not positive!");
        static_assert(std::is_base_of<SuperCubeData<SpatialDimensions>, T>::value,
                      "Template parameter T is not a subclass of SuperCubeData<SpatialDimensions>!");

        using IndexData = Eigen::Matrix<double, SpatialDimensions, 1>;
        using Data = T;

        SuperCubeInterface() = default;

        virtual ~SuperCubeInterface() = default;

        void setData(Data const &p, bool verbose = false) {
            IndexData f = p.getIndex();
            this->setData(f, p, verbose);
        }

        void setData(IndexData f, Data const &p, bool verbose = false) {
            this->setData(std::move(f), p, verbose, "");
        }

        virtual void setData(IndexData f, Data const &p, bool verbose, std::string output) = 0;

        void saveBinary(std::string const &filename) const {
            std::ofstream bin(filename, std::ios::binary | std::ios::out);
            saveBinary(bin);
        }

        void readBinary(std::string const &filename) {
            std::ifstream bin(filename, std::ios::binary | std::ios::in);
            readBinary(bin);
        }

        virtual void saveBinary(std::ofstream &bin) const = 0;

        virtual void readBinary(std::ifstream &bin) = 0;

    protected:
        static int indexFromData(IndexData &data, int spaceDivision, bool preserveData = false) {
            int index = 0;
            int powDivision = 1;
            for (int i = SpatialDimensions - 1; i >= 0; i--) {
                if (data[i] < 0) {
                    std::cout << data.transpose() << std::endl;
                    throw std::runtime_error(
                            "Data contains negative elements; it shouldn't for computing unique indices!");
                }
                int p_floor = int(floor(data[i]));
                if (!preserveData) {
                    data[i] -= p_floor;
                }
                index += powDivision * p_floor;
                powDivision *= spaceDivision;
            }
            if (!preserveData) {
                data *= spaceDivision;
            }
            return index;
        }
    };

    template<typename T, int SpatialDimension, int SpatialDivision, int Depth>
    class SuperCube : public SuperCubeInterface<T, SpatialDimension, SpatialDivision> {
        static_assert(Depth > 0, "Depth of the SuperCube is negative!");
        static_assert(SpatialDivision > 0, "SpatialDivision of the SuperCube is negative!");

        using Base = SuperCubeInterface<T, SpatialDimension, SpatialDivision>;
        using Base::indexFromData;
    public:
        using IndexData = typename Base::IndexData;
        using Data = typename Base::Data;
        using Base::setData;
        using Base::saveBinary;
        using Base::readBinary;
        friend nlohmann::adl_serializer<SuperCube<T, SpatialDimension, SpatialDivision, Depth>, void>;

        SuperCube<T, SpatialDimension, SpatialDivision, Depth + 1> *parent;

        explicit SuperCube(SuperCube<T, SpatialDimension, SpatialDivision, Depth + 1> *parent = nullptr) :
                parent(parent) {}

        void setData(IndexData f, Data const &p, bool verbose, std::string output) override {
            int i = indexFromData(f, SpatialDivision);
            SuperCube<T, SpatialDimension, SpatialDivision, Depth - 1> *storedData;
            if (verbose) {
                output += "i = " + std::to_string(i) + "; ";
            }
            if (!mapGetIfContains(this->data, i, storedData)) {
                SuperCube<T, SpatialDimension, SpatialDivision, Depth - 1> s(this);
                s.setData(f, p, verbose, output);
                this->data[i] = s;
                return;
            }
            storedData->setData(f, p, verbose, output);
        }

        void saveBinary(std::ofstream &bin) const override {
            serialize(bin, (int) this->data.size());  // has to be an int: max value = 1e6
            for (auto const &mapData: this->data) {
                serialize(bin, mapData.first);
                mapData.second.saveBinary(bin);
            }
        }

        void readBinary(std::ifstream &bin) override {
            try {
                int mapSize;
                deserialize(bin, mapSize);
                this->data.clear();
                for (int i = 0; i < mapSize; i++) {
                    int index;
                    deserialize(bin, index);
                    SuperCube<T, SpatialDimension, SpatialDivision, Depth - 1> s(this);
                    s.readBinary(bin);
                    this->data[index] = s;
                }
            } catch (std::runtime_error &e) {
                throw std::runtime_error("Binary file is corrupted...");;
            }
        }

    protected:
        std::map<int, SuperCube<T, SpatialDimension, SpatialDivision, Depth - 1>> data;
    };

    template<typename T, int SpatialDimension, int SpatialDivision>
    class SuperCube<T, SpatialDimension, SpatialDivision, 0>
            : public SuperCubeInterface<T, SpatialDimension, SpatialDivision> {
        using Base = SuperCubeInterface<T, SpatialDimension, SpatialDivision>;
        using Base::indexFromData;
    public:
        using IndexData = typename Base::IndexData;
        using Data = typename Base::Data;
        using Base::setData;
        using Base::saveBinary;
        using Base::readBinary;
        friend nlohmann::adl_serializer<SuperCube<T, SpatialDimension, SpatialDivision, 0>, void>;

        SuperCube<T, SpatialDimension, SpatialDivision, 1> *parent;

        explicit SuperCube(SuperCube<T, SpatialDimension, SpatialDivision, 1> *parent = nullptr) : parent(parent) {}

        void setData(IndexData f, Data const &p, bool verbose, std::string output) override {
            int i = indexFromData(f, SpatialDivision);
            Data *storedData;
            if (!mapGetIfContains(this->data, i, storedData)) {
                if (verbose) {
                    std::cout << output << "i = " << i << std::endl;
                }
                this->data[i] = p;
                return;
            }
            storedData->update(p);
        }

        void saveBinary(std::ofstream &bin) const override {
            serialize(bin, (int) this->data.size());  // has to be an int: max value = 1e6
            for (auto const &mapData: this->data) {
                serialize(bin, mapData.first);
                mapData.second.saveBinary(bin);
            }
        }

        void readBinary(std::ifstream &bin) override {
            try {
                int mapSize;
                deserialize(bin, mapSize);
                this->data.clear();
                for (int i = 0; i < mapSize; i++) {
                    int index;
                    deserialize(bin, index);
                    Data s;
                    s.readBinary(bin);
                    this->data[index] = s;
                }
            } catch (std::runtime_error &e) {
                throw std::runtime_error("Binary file is corrupted...");;
            }
        }

    protected:
        std::map<int, Data> data;
    };
}

#endif //ANDREIUTILS_SUPERCUBE_HPP
