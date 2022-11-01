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

    class SuperCubeOutOfRangeException : std::runtime_error {
    public:
        SuperCubeOutOfRangeException() : std::runtime_error("Out of SuperCube representation range!") {}
    };

    template<typename T, int SpatialDimensions, int SpatialDivision, int Depth>
    class SuperCubeInterface {
    public:
        static_assert(Depth >= 0, "Depth of the SuperCube is negative!");
        static_assert(SpatialDivision > 0, "SpatialDivision of the SuperCube is not positive!");
        static_assert(SpatialDimensions > 0, "SpatialDimensions of the SuperCube is not positive!");
        static_assert(std::is_base_of<SuperCubeData<SpatialDimensions>, T>::value,
                      "Template parameter T is not a subclass of SuperCubeData<SpatialDimensions>!");

        using Data = T;
        using DataIndex = Eigen::Matrix<double, SpatialDimensions, 1>;
        using DimensionIndex = Eigen::Matrix<int, SpatialDimensions, 1>;

        SuperCubeInterface(int const &parentIndex, DataIndex const &minCorner, DataIndex const &maxCorner) :
                parentIndex(parentIndex), maxCorner(maxCorner), minCorner(minCorner), volume(maxCorner - minCorner),
                nrCubes(pow(SuperCubeInterface::division, SuperCubeInterface::dimensions)) {
            for (int i = 0; i < SuperCubeInterface::dimensions; i++) {
                this->size[i] = SpatialDivision;
            }
            this->computeSubCubeVolume();
            /*
            std::cout << this->minCorner.transpose() << "; " << this->maxCorner.transpose() << "; "
                      << this->volume.transpose() << std::endl;
            std::cout << this->size.transpose() << std::endl;
            std::cout << this->subCubeVolume.transpose() << std::endl;
            //*/
        }

        SuperCubeInterface(int const &parentIndex, DataIndex const &minCorner, DataIndex const &maxCorner,
                           DimensionIndex const &nrCubesPerDimension) :
                parentIndex(parentIndex), maxCorner(maxCorner), minCorner(minCorner), volume(maxCorner - minCorner),
                size(nrCubesPerDimension), nrCubes(1) {
            for (int i = 0; i < SuperCubeInterface::dimensions; i++) {
                this->nrCubes *= this->size[i];
            }
            this->computeSubCubeVolume();
            /*
            std::cout << this->minCorner.transpose() << "; " << this->maxCorner.transpose() << "; "
                      << this->volume.transpose() << std::endl;
            std::cout << this->size.transpose() << std::endl;
            std::cout << this->subCubeVolume.transpose() << std::endl;
            //*/
        }

        virtual ~SuperCubeInterface() = default;

        void setData(Data const &p, bool verbose = false) {
            DataIndex f = p.getIndex() - this->minCorner;  // ensure that the index is inside the cube!
            this->setData(f, p, verbose);
        }

        void setData(DataIndex const &f, Data const &p, bool verbose = false) {
            this->setData(f, p, verbose, "");
        }

        virtual void setData(DataIndex const &f, Data const &p, bool verbose, std::string output) = 0;

        virtual Data &getData(DimensionIndex f) = 0;

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

        int const &getNrCubes() const {
            return this->nrCubes;
        }

        DataIndex const &getVolume() const {
            return this->volume;
        }

        DataIndex const &getSubCubeVolume() const {
            return this->subCubeVolume;
        }

        DataIndex const &getCubeMinCorner() const {
            return this->minCorner;
        }

        DataIndex const &getCubeMaxCorner() const {
            return this->maxCorner;
        }

        DimensionIndex const &getSubCubeDivision() const {
            return this->size;
        }

    protected:
        static int const dimensions = SpatialDimensions, division = SpatialDivision, depth = Depth;
        DataIndex minCorner, maxCorner, volume, subCubeVolume;  // min = inclusive, max = exclusive
        DimensionIndex size;  // each dimension from 0 until size[dim] - 1
        int nrCubes, parentIndex;
        Data data;

        void computeSubCubeVolume() {
            this->subCubeVolume = ((this->maxCorner - this->minCorner).array().cwiseQuotient(
                    this->size.template cast<double>().array()));
        }

        DataIndex getSubCubeMinCorner(DimensionIndex const &dimensionIndex) {
            /*
            std::cout << "getSubCubeMinCorner:" << std::endl;
            std::cout << this->minCorner.transpose() << std::endl;
            std::cout << dimensionIndex.transpose() << std::endl;
            std::cout << this->subCubeVolume.transpose() << std::endl;
            auto tmp = (dimensionIndex.template cast<double>().array().
                    template cwiseProduct(this->subCubeVolume.array())).matrix();
            std::cout << tmp.transpose() << std::endl;
            std::cout << (this->minCorner + tmp).transpose() << std::endl;
            //*/
            return this->minCorner + (dimensionIndex.template cast<double>().array().
                    template cwiseProduct(this->subCubeVolume.array())).matrix();
        }

        DataIndex getSubCubeMinCorner(DataIndex const &dataIndex) {
            return this->getSubCubeMinCorner(this->convertDataToIndex(dataIndex));
        }

        DataIndex getSubCubeMinCorner(int const &cubeIndex) {
            return this->getSubCubeMinCorner(this->localIndexFromCubeIndex(cubeIndex));
        }

        DimensionIndex convertDataToIndex(DataIndex const &dataIndex) {
            return ((dataIndex - this->minCorner).array().cwiseQuotient(
                    this->subCubeVolume.array())).template cast<int>();
        }

        int cubeIndexFromData(DataIndex const &dataIndex) const {
            int cubeIndex = 0, powDivision = 1;
            // DimensionIndex index;
            for (int i = SuperCubeInterface::dimensions - 1; i >= 0; i--) {
                if (dataIndex[i] < this->minCorner[i] || dataIndex[i] >= this->maxCorner[i]) {
                    std::cout << dataIndex.transpose() << ", min = " << this->minCorner.transpose() << ", max = "
                              << this->maxCorner.transpose() << std::endl;
                    throw SuperCubeOutOfRangeException();
                }
                int pFloor = int(floor((dataIndex[i] - this->minCorner[i]) / this->subCubeVolume[i]));
                // index[i] = pFloor;
                cubeIndex += powDivision * pFloor;
                powDivision *= this->size[i];
            }
            // std::cout << index.transpose() << std::endl;
            return cubeIndex;
        }

        DimensionIndex localIndexFromCubeIndex(int cubeIndex) {
            DimensionIndex localIndex;
            for (int i = SuperCubeInterface::dimensions - 1; i >= 0; i--) {
                localIndex[i] = cubeIndex % this->size[i];
                cubeIndex /= this->size[i];
            }
            return localIndex;
        }

        int cubeIndexFromLocalIndex(DimensionIndex const &localIndex) const {
            int cubeIndex = 0, powDivision = 1;
            for (int i = SuperCubeInterface::dimensions - 1; i >= 0; i--) {
                if (localIndex[i] < 0 || localIndex[i] >= this->size[i]) {
                    std::cout << localIndex.transpose() << ", min = " << DataIndex::Zero().transpose() << ", max = "
                              << this->size.transpose() << std::endl;
                    throw SuperCubeOutOfRangeException();
                }
                cubeIndex += powDivision * localIndex[i];
                powDivision *= this->size[i];
            }
            return cubeIndex;
        }
    };

    template<typename T, int SpatialDimension, int SpatialDivision, int Depth = 1>
    class SuperCube : public SuperCubeInterface<T, SpatialDimension, SpatialDivision, Depth> {
        using Base = SuperCubeInterface<T, SpatialDimension, SpatialDivision, Depth>;
        using Base::cubeIndexFromData;
        using TopCube = SuperCube<T, SpatialDimension, SpatialDivision, Depth + 1>;
        using TopCubeInterface = SuperCubeInterface<T, SpatialDimension, SpatialDivision, Depth + 1>;
    public:
        using SubCube = SuperCube<T, SpatialDimension, SpatialDivision, Depth - 1>;
        using DataIndex = typename Base::DataIndex;
        using DimensionIndex = typename Base::DimensionIndex;
        using Data = typename Base::Data;
        using Base::setData;
        using Base::saveBinary;
        using Base::readBinary;
        friend nlohmann::adl_serializer<SuperCube<T, SpatialDimension, SpatialDivision, Depth>, void>;
        friend TopCube;

        explicit SuperCube(DataIndex const &minCorner, DataIndex const &maxCorner) :
                Base(0, minCorner, maxCorner), parent(nullptr) {}

        explicit SuperCube(DimensionIndex const &nrCubesPerDimension, DataIndex const &minCorner,
                           DataIndex const &maxCorner) :
                Base(0, minCorner, maxCorner, nrCubesPerDimension), parent(nullptr) {}

        void setData(DataIndex const &f, Data const &p, bool verbose, std::string output) override {
            int i = this->cubeIndexFromData(f);
            /*
            std::cout << "At Depth: " << Depth << std::endl;
            std::cout << "Cube Index from Data: " << i << std::endl;
            std::cout << "Data Index = " << f.transpose() << std::endl;
            //*/
            SubCube *storedData = nullptr;
            if (verbose) {
                output += "i = " + std::to_string(i) + "; ";
            }
            if (!this->subCubes.getIfContains(i, storedData)) {
                // std::cout << "Adding new sub cube..." << std::endl;
                DataIndex subCubeMinCorner = this->getSubCubeMinCorner(f);
                // std::cout << "subCubeMinCorner = " << subCubeMinCorner.transpose() << std::endl;
                storedData = &this->subCubes.set(
                        i, {*this, i, subCubeMinCorner, subCubeMinCorner + this->subCubeVolume});
            }
            storedData->setData(f, p, verbose, output);
        }

        Data &getData(DimensionIndex f) override {
            ;
        }

        std::map<int, SubCube> const &getAllData() const {
            return this->subCubes.subCubes;
        }

        void saveBinary(std::ofstream &bin) const override {
            serialize(bin, (int) this->subCubes.subCubes.size());  // has to be an int: max value = 1e6
            for (auto const &mapData: this->getAllData()) {
                serialize(bin, mapData.first);
                mapData.second.saveBinary(bin);
            }
        }

        void readBinary(std::ifstream &bin) override {
            try {
                int mapSize;
                deserialize(bin, mapSize);
                this->subCubes.subCubes.clear();
                for (int i = 0; i < mapSize; i++) {
                    int index;
                    deserialize(bin, index);
                    DataIndex subCubeMinCorner = this->getSubCubeMinCorner(index);
                    this->subCubes.set(index, {*this, i, subCubeMinCorner,
                                               subCubeMinCorner + this->subCubeVolume}).readBinary(bin);
                }
            } catch (std::runtime_error &e) {
                throw std::runtime_error("Binary file is corrupted...");;
            }
        }

    protected:
        SuperCube(TopCube &parent, int const &parentIndex, DataIndex const &minCorner, DataIndex const &maxCorner) :
                Base(parentIndex, minCorner, maxCorner), parent(&parent) {}

        class SuperCubeSubCubeContainer {
        public:
            friend SuperCube;
            friend nlohmann::adl_serializer<SuperCube, void>;

            SuperCubeSubCubeContainer() = default;

            virtual ~SuperCubeSubCubeContainer() { // NOLINT(modernize-use-equals-default)
                /*
                for (auto const &subCubeData: this->subCubes) {
                    delete subCubeData.second;
                }
                //*/
            }

            SubCube &set(int const &subCubeIndex, SubCube const &subCube) {
                return mapAdd(this->subCubes, subCubeIndex, subCube)->second;
            }

            SubCube &set(int const &subCubeIndex, SubCube &&subCube) {
                // return subCube;
                // return this->subCubes.insert(std::make_pair(subCubeIndex, std::forward<SubCube>(subCube))).first->second;
                // auto x = this->subCubes.insert(std::make_pair(subCubeIndex, std::forward<SubCube>(subCube)));
                // return x.first->second;
                return mapAdd(this->subCubes, subCubeIndex, std::move(subCube))->second;
            }

            SubCube const &getAtIndex(int const &subCubeIndex) const {
                return mapGet(this->subCubes, subCubeIndex);
            }

            SubCube &getAtIndex(int const &subCubeIndex) {
                return mapGet(this->subCubes, subCubeIndex);
            }

            bool getIfContains(int const &subCubeIndex, SubCube *&storedData) {
                return mapGetIfContains(this->subCubes, subCubeIndex, storedData);
            }

            bool getIfContains(int const &subCubeIndex, SubCube const *&storedData) const {
                return mapGetIfContains(this->subCubes, subCubeIndex, storedData);
            }

        protected:
            std::map<int, SubCube> subCubes;
        };

        SuperCubeSubCubeContainer subCubes;
        TopCubeInterface *parent;
    };

    template<typename T, int SpatialDimension, int SpatialDivision>
    class SuperCube<T, SpatialDimension, SpatialDivision, 0>
            : public SuperCubeInterface<T, SpatialDimension, SpatialDivision, 0> {
        using Base = SuperCubeInterface<T, SpatialDimension, SpatialDivision, 0>;
        using Base::cubeIndexFromData;
        using TopCube = SuperCube<T, SpatialDimension, SpatialDivision, 1>;
        using TopCubeInterface = SuperCubeInterface<T, SpatialDimension, SpatialDivision, 1>;
    public:
        using DataIndex = typename Base::DataIndex;
        using DimensionIndex = typename Base::DimensionIndex;
        using Data = typename Base::Data;
        using Base::setData;
        using Base::saveBinary;
        using Base::readBinary;
        friend nlohmann::adl_serializer<SuperCube<T, SpatialDimension, SpatialDivision, 0>, void>;
        friend TopCube;

        void setData(DataIndex const &f, Data const &p, bool verbose, std::string output) override {
            int i = this->cubeIndexFromData(f);
            if (i != 0) {
                throw SuperCubeOutOfRangeException();
            }
            this->data.update(p);
        }

        Data &getData(DimensionIndex f) override {
            if (f == DimensionIndex::Zero()) {
                return this->data;
            }
            if (this->parent == nullptr) {
                throw SuperCubeOutOfRangeException();
            }
            // TODO: process neighbor index!
        }

        std::map<int, Data> getAllData() const {
            return {{0, this->data}};
        }

        void saveBinary(std::ofstream &bin) const override {
            this->data.saveBinary(bin);
        }

        void readBinary(std::ifstream &bin) override {
            try {
                this->data.readBinary(bin);
            } catch (std::runtime_error &e) {
                throw std::runtime_error("Binary file is corrupted...");;
            }
        }

    protected:
        SuperCube(TopCube &parent, int const &parentIndex, DataIndex const &minCorner, DataIndex const &maxCorner) :
                Base(parentIndex, minCorner, maxCorner, DimensionIndex::Ones()), parent(&parent) {}

        TopCubeInterface *parent;
    };
}

#endif //ANDREIUTILS_SUPERCUBE_HPP
