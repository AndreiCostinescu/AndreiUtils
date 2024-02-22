//
// Created by Andrei on 03.11.22.
//

#include <AndreiUtils/classes/RandomNumberGenerator.hpp>
#include <AndreiUtils/classes/Timer.hpp>
#include <AndreiUtils/classes/SuperCube.hpp>
#include <AndreiUtils/utilsJsonEigen.hpp>
#include <iomanip>

using namespace AndreiUtils;
using namespace std;

class TmpClass : public SuperCubeData<3> {
public:
    Eigen::Vector3d v;

    TmpClass() : v(-1 * Eigen::Vector3d::Ones()) {}

    TmpClass(double x, double y, double z) : v({x, y, z}) {}

    IndexType getIndex() const override {
        if (v.x() < 0 && v.y() < 0 && v.z() < 0) {
            RandomNumberGenerator<double> r(0, 20);
            IndexType sample{r.sample(), r.sample(), r.sample()};
            cout << "Sample: " << sample.transpose() << endl;
            return sample;
        }
        return v;
    }

    void saveBinary(std::ofstream &bin) const override {
        ;
    }

    void readBinary(std::ifstream &bin) override {
        ;
    }

    void update(TmpClass const &tmp) {
        cout << "Called update on data: " << this->v.transpose() << " with update " << tmp.v.transpose() << endl;
        this->v = tmp.v;
    }
};

namespace nlohmann {
    template<>
    struct adl_serializer<TmpClass> {
        static void to_json(nlohmann::json &j, const TmpClass &v) {}

        static void from_json(const nlohmann::json &j, TmpClass &v) {}
    };
}

class TSDFData : public AndreiUtils::SuperCubeData<3> {
public:
    Eigen::Vector3d p;
    double sdf, weight, weightLimit;

    TSDFData() : p(0, 0, 0), sdf(1.0), weight(0.0), weightLimit(64) {}

    TSDFData(Eigen::Vector3d p, double const &sdf, double const &weight = 1.0, double const &weightLimit = 64.0) :
            p(std::move(p)), sdf(sdf), weight(weight), weightLimit(weightLimit) {}

    [[nodiscard]] IndexType getIndex() const override {
        return this->p;
    }

    void saveBinary(std::ofstream &bin) const override {
        serialize(bin, this->p.data(), 3);
        serialize(bin, this->sdf);
        serialize(bin, this->weight);
    }

    void readBinary(std::ifstream &bin) override {
        deserializeIn(bin, this->p.data(), 3);
        deserializeIn(bin, this->sdf);
        deserializeIn(bin, this->weight);
    }

    void update(TSDFData const &other) {
        this->sdf = (this->weight * this->sdf + other.sdf * other.weight) / (this->weight + other.weight);
        this->weight = (this->weight < weightLimit) ? this->weight + other.weight : this->weightLimit;
    }
};

void representSphere(SuperCube<TSDFData, 3, 10, 3> &s, double const &radius, double const &sdfSub = 0.5) {
    cout << pow((2 * radius / 0.005), 3) << endl;
    // return;
    Timer t;
    double time, maxTime = 0;
    for (double x = -radius; x < radius; x += 0.005) {  // NOLINT(cert-flp30-c)
        for (double y = -radius; y < radius; y += 0.005) {  // NOLINT(cert-flp30-c)
            for (double z = -radius; z < radius; z += 0.005) {  // NOLINT(cert-flp30-c)
                double sdf = Eigen::Vector3d(x, y, z).norm() - sdfSub;
                t.start();
                s.setData({{x, y, z}, sdf});
                time = t.measure("ms");
                if (time > maxTime) {
                    maxTime = time;
                    cout << "Set Data in " << time << " ms" << endl;
                }
            }
        }
    }
    cout << "#Top-Level Cubes:  " << s.getAllData().size() << endl;
}

template<typename T, int Dim, int Div, int Depth>
void printSuperCubeData(string const &preText, int index, SuperCube<T, Dim, Div, Depth> const &s) {
    cout << "At " << preText << " = " << index << ": " << s.getNrCubes() << "; " << s.getVolume().transpose()
         << "; " << s.getSubCubeVolume().transpose() << "; " << s.getCubeMinCorner().transpose() << "; "
         << s.getCubeMaxCorner().transpose() << endl;
}

void testSuperCube() {
    using S = SuperCube<TmpClass, 3, 10, 3>;
    S::DataIndex a = 10 * S::DataIndex::Ones(), b = S::DataIndex::Ones(), res;
    S::DimensionIndex c = S::DimensionIndex::Ones();
    S::DataIndex tmp = (a - b).cwiseQuotient(c.template cast<double>());
    res = ((a - b).array() / c.cast<double>().array()).matrix();
    cout << res.transpose() << endl;

    //*
    S x({10, 5, 2}, {0, 0, 0}, {20, 20, 20});

    x.saveBinary("123.txt");
    x.setData(TmpClass());
    x.setData(TmpClass());
    x.setData(TmpClass());
    x.setData(TmpClass());

    printSuperCubeData("Index-3", 0, x);
    for (auto const &data_2: x.getAllData()) {
        printSuperCubeData("Index-2", data_2.first, data_2.second);
        for (auto const &data_1: data_2.second.getAllData()) {
            printSuperCubeData("Index-1", data_1.first, data_1.second);
            for (auto const &data_0: data_1.second.getAllData()) {
                printSuperCubeData("Index-0", data_0.first, data_0.second);
                for (auto const &data: data_0.second.getAllData()) {
                    cout << "At Data Level = " << data.first << ": OK" << endl;
                }
            }
        }
    }

    SuperCube<TmpClass, 3, 3, 2> y({0, 0, 0}, {9, 9, 9});
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            for (int k = 0; k < 9; k++) {
                y.setData(TmpClass(i, j, k));
            }
        }
    }

    printSuperCubeData("Index-2", 0, y);
    for (auto const &data_1: y.getAllData()) {
        printSuperCubeData("Index-1", data_1.first, data_1.second);
        for (auto const &data_0: data_1.second.getAllData()) {
            printSuperCubeData("Index-0", data_0.first, data_0.second);
            for (auto const &data: data_0.second.getAllData()) {
                cout << "At Data Level = " << data.first << ": " << data.second.v.transpose() << endl;
                try {
                    TmpClass const &neighborData = data_0.second.getData(S::DimensionIndex{0, 0, 1});
                    cout << "Z-Neighbor Data: " << neighborData.v.transpose() << endl;
                } catch (SuperCubeOutOfRangeException &e) {
                    cout << "Next neighbor in z-direction is out of range!" << endl;
                }
                try {
                    TmpClass const &neighborData = data_0.second.getData(S::DimensionIndex{0, 1, 0});
                    cout << "Y-Neighbor Data: " << neighborData.v.transpose() << endl;
                } catch (SuperCubeOutOfRangeException &e) {
                    cout << "Next neighbor in y-direction is out of range!" << endl;
                }
                try {
                    TmpClass const &neighborData = data_0.second.getData(S::DimensionIndex{1, 0, 0});
                    cout << "X-Neighbor Data: " << neighborData.v.transpose() << endl;
                } catch (SuperCubeOutOfRangeException &e) {
                    cout << "Next neighbor in x-direction is out of range!" << endl;
                }
            }
        }
    }

    // SuperCube<TmpClass, 3, 1, -1> z;
    // SuperCube<TmpClass, 3, 0, 1> z;
    // SuperCube<TmpClass, 0, 1, 1> z;

    nlohmann::json j = x;
    //*/
}

void testSuperCubeSphere() {
    cout << setprecision(16);

    using S = SuperCube<TSDFData, 3, 10, 3>;
    S x({200, 200, 20}, {-100, -100, -10}, {100, 100, 10});
    representSphere(x, 0.8);

    printSuperCubeData("Index-3", 0, x);
    for (auto const &data_2: x.getAllData()) {
        printSuperCubeData("Index-2", data_2.first, data_2.second);
        for (auto const &data_1: data_2.second.getAllData()) {
            printSuperCubeData("Index-1", data_1.first, data_1.second);
            for (auto const &data_0: data_1.second.getAllData()) {
                printSuperCubeData("Index-0", data_0.first, data_0.second);
                for (auto const &data: data_0.second.getAllData()) {
                    cout << "At Data Level = " << data.first << ": OK" << endl;
                }
            }
        }
    }
}

int main() {
    cout << "Hello World!" << endl;

    // testSuperCube();
    testSuperCubeSphere();

    return 0;
}
