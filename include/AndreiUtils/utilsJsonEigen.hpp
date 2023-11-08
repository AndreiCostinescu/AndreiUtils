//
// Created by Andrei on 27.08.21.
//

#pragma once

#include <AndreiUtils/classes/DualQuaternion.hpp>
#include <AndreiUtils/classes/SuperCube.hpp>
#include <AndreiUtils/classes/Symmetry.h>
#include <AndreiUtils/classes/motion/MotionDeviceCaptureParameters.h>
#include <AndreiUtils/classes/motion/MotionDeviceIntrinsicParameters.h>
#include <AndreiUtils/json.hpp>
#include <AndreiUtils/utilsGeometry.h>
#include <AndreiUtils/utilsJson.h>
#include <complex>
#include <Eigen/Dense>
#include <string>
#include <vector>

namespace nlohmann {
    template<class T>
    struct adl_serializer<Eigen::Matrix<T, 2, 1>> {
        static void to_json(nlohmann::json &j, const Eigen::Matrix<T, 2, 1> &v) {
            j = std::vector<double>{v(0), v(1)};
        }

        static void from_json(const nlohmann::json &j, Eigen::Matrix<T, 2, 1> &v) {
            std::vector<double> _v = j.get<std::vector<double>>();
            v(0) = _v[0];
            v(1) = _v[1];
        }
    };

    template<class T>
    struct adl_serializer<Eigen::Matrix<T, 3, 1>> {
        static void to_json(nlohmann::json &j, const Eigen::Matrix<T, 3, 1> &v) {
            j = std::vector<double>{v(0), v(1), v(2)};
        }

        static void from_json(const nlohmann::json &j, Eigen::Matrix<T, 3, 1> &v) {
            std::vector<double> _v = j.get<std::vector<double>>();
            v(0) = _v[0];
            v(1) = _v[1];
            v(2) = _v[2];
        }
    };

    template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    struct adl_serializer<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
        using T = Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>;

        static std::string getStringFromType() {
            if (std::is_same<Scalar, int>::value) {
                return "i";
            } else if (std::is_same<Scalar, float>::value) {
                return "f";
            } else if (std::is_same<Scalar, double>::value) {
                return "d";
            } else if (std::is_same<Scalar, std::complex<float>>::value) {
                return "cf";
            } else if (std::is_same<Scalar, std::complex<double>>::value) {
                return "cd";
            } else {
                throw std::runtime_error("Unknown scalar type: " + std::string(typeid(Scalar).name()));
            }
        }

        static void to_json(nlohmann::json &j, T const &data) {
            int currentRows = data.rows(), currentCols = data.cols();

            j["type"] = getStringFromType();
            j["rows"] = Rows;
            j["cols"] = Cols;
            j["curRows"] = currentRows;
            j["curCols"] = currentCols;
            j["options"] = Options;
            j["maxRows"] = MaxRows;
            j["maxCols"] = MaxCols;

            typedef std::vector<Scalar> V;
            V jsonData(currentRows * currentCols);
            for (int r = 0; r < currentRows; r++) {
                for (int c = 0; c < currentCols; c++) {
                    jsonData[r * currentCols + c] = data(r, c);
                }
            }
            j["data"] = jsonData;
        }

        static void from_json(nlohmann::json const &j, T &data) {
            if (!(j.template contains("type") && j.template contains("rows") && j.template contains("cols") &&
                  j.template contains("curRows") && j.template contains("curCols") && j.template contains("options") &&
                  j.template contains("maxRows") && j.template contains("maxCols") && j.template contains("data"))) {
                if (j.is_array()) {
                    std::vector<Scalar> vectorData = j.get<std::vector<Scalar>>();
                    size_t nrVectorElements = vectorData.size();
                    if (Rows != -1 && Cols != -1) {
                        if (Rows * Cols != nrVectorElements) {
                            throw std::runtime_error(
                                    "Unable to deserialize json array data into an Eigen Matrix (" +
                                    std::to_string(Rows) + " x " + std::to_string(Cols) + "): " + j.dump());
                        }
                        std::cout << "Deserializing " << nrVectorElements << " into " << Rows * Cols << " elements" << std::endl;
                        for (Eigen::Index iIndex = 0; iIndex < Rows; ++iIndex) {
                            for (Eigen::Index jIndex = 0; jIndex < Cols; ++jIndex) {
                                data(iIndex, jIndex) = vectorData[iIndex * Cols + jIndex];
                            }
                        }
                    } else if (Rows == -1 && Cols == -1) {
                        data = T::Zero((Eigen::Index) nrVectorElements, 1);
                        for (Eigen::Index iIndex = 0; iIndex < nrVectorElements; ++iIndex) {
                            data(iIndex) = vectorData[iIndex];
                        }
                    } else if (Rows == -1) {
                        int nrRows = nrVectorElements / Cols;
                        if (Cols * nrRows != nrVectorElements) {
                            throw std::runtime_error(
                                    "Unable to deserialize json array data into a variable-row Eigen Matrix (" +
                                    std::to_string(Rows) + " x " + std::to_string(Cols) + "): " + j.dump());
                        }
                        data = T::Zero(nrRows, Cols);
                        for (Eigen::Index iIndex = 0; iIndex < nrVectorElements; ++iIndex) {
                            data(iIndex) = vectorData[iIndex];
                        }
                    } else {
                        assert(Cols == -1);
                        int nrCols = nrVectorElements / Rows;
                        if (Rows * nrCols != nrVectorElements) {
                            throw std::runtime_error(
                                    "Unable to deserialize json array data into a variable-columns Eigen Matrix (" +
                                    std::to_string(Rows) + " x " + std::to_string(Cols) + "): " + j.dump());
                        }
                        data = T::Zero(Rows, nrCols);
                        for (Eigen::Index iIndex = 0; iIndex < nrVectorElements; ++iIndex) {
                            data(iIndex) = vectorData[iIndex];
                        }
                    }
                    return;
                }
                throw std::runtime_error("Poorly formatted json data for EigenMatrixJsonSerializer: " + j.dump());
            }

            if (getStringFromType() != j["type"].get<std::string>()) {
                throw std::runtime_error("Requested matrix type is different than the stored matrix type");
            }

            int rows = j["rows"].get<int>();
            int currentRows = j["curRows"].get<int>();
            if (Rows != -1) {
                if (rows != -1 && Rows != rows) {
                    throw std::runtime_error("Requested matrix rows is different than the stored matrix rows");
                } else if (rows == -1 && Rows != currentRows) {
                    throw std::runtime_error("Requested matrix rows is different than the current matrix rows");
                }
            }

            int cols = j["cols"].get<int>();
            int currentCols = j["curCols"].get<int>();
            if (Cols != -1) {
                if (cols != -1 && Cols != cols) {
                    throw std::runtime_error("Requested matrix columns is different than the stored matrix columns");
                } else if (cols == -1 && Cols != currentCols) {
                    throw std::runtime_error("Requested matrix columns is different than the current matrix columns");
                }
            }

            if (Options != j["options"].get<int>()) {
                throw std::runtime_error("Requested matrix options is different than the stored matrix options");
            }

            int maxRows = j["maxRows"].get<int>();
            if (MaxRows != -1 && maxRows != -1 && MaxRows != maxRows) {
                throw std::runtime_error("Requested matrix max rows is different than the stored matrix max rows");
            }

            int maxCols = j["maxCols"].get<int>();
            if (MaxCols != -1 && maxCols != -1 && MaxCols != maxCols) {
                throw std::runtime_error(
                        "Requested matrix max columns is different than the stored matrix max columns");
            }

            typedef std::vector<Scalar> V;
            V jsonData = j["data"].get<V>();
            data.setZero(currentRows, currentCols);
            for (int r = 0; r < currentRows; r++) {
                for (int c = 0; c < currentCols; c++) {
                    data(r, c) = jsonData[r * currentCols + c];
                }
            }
        }
    };

    template<class T>
    struct adl_serializer<Eigen::Quaternion<T>> {
        static void to_json(nlohmann::json &j, Eigen::Quaternion<T> const &q) {
            std::vector<T> coefficients{q.w(), q.x(), q.y(), q.z()};
            j = coefficients;
        }

        static void from_json(nlohmann::json const &j, Eigen::Quaternion<T> &q) {
            auto coefficients = j.get<std::vector<T>>();
            if (coefficients.size() != 4) {
                throw std::runtime_error(
                        "Coefficients' size is not 4 (is " + std::to_string(coefficients.size()) + ")!");
            }
            q.w() = coefficients[0];
            q.x() = coefficients[1];
            q.y() = coefficients[2];
            q.z() = coefficients[3];
        }
    };

    template<class T>
    struct adl_serializer<AndreiUtils::DualQuaternion<T>> {
        static void to_json(nlohmann::json &j, AndreiUtils::DualQuaternion<T> const &q) {
            j = q.coefficients();
        }

        static void from_json(nlohmann::json const &j, AndreiUtils::DualQuaternion<T> &q) {
            if (!j.is_array()) {
                throw std::runtime_error("Can't convert non-arrays to poses");
            }
            if (j.size() == 8 && j[0].is_number()) {
                q.fromCoefficients(j.get<std::vector<T>>());
                return;
            }
            q = AndreiUtils::Posed::one;
            for (auto const &jIntern: j.get<std::vector<nlohmann::json>>()) {
                if (!jIntern.is_array()) {
                    throw std::runtime_error("Can't convert non-internal-arrays to poses");
                }
                if (jIntern.size() != 3 && jIntern.size() != 4) {
                    throw std::runtime_error("Unknown format for pose-composition data!");
                }
                if (jIntern.size() == 4) {
                    // Assume quaternion components
                    q = q.addRotationRight(Eigen::Quaterniond{jIntern[0].get<double>(), jIntern[1].get<double>(),
                                                              jIntern[2].get<double>(), jIntern[3].get<double>()});
                } else if (jIntern[0].is_number()) {
                    q = q.addTranslation(
                            {jIntern[0].get<double>(), jIntern[1].get<double>(), jIntern[2].get<double>()});
                } else if (!jIntern[0].is_string()) {
                    throw std::runtime_error("Can't convert non-string-starting arrays to orientations");
                } else {
                    double angle;
                    if (jIntern[0].get<std::string>() == "d") {
                        angle = AndreiUtils::deg2Rad(jIntern[1].get<double>());
                    } else if (jIntern[0].get<std::string>() == "r") {
                        angle = jIntern[1].get<double>();
                    } else {
                        throw std::runtime_error("Unknown angle-axis angle format specification!");
                    }
                    Eigen::Vector3d axis;
                    if (!jIntern[2].is_array() || jIntern[2].size() != 3) {
                        throw std::runtime_error("Can't convert angle-axis axis if it's not a 3-dim vector data!");
                    }
                    axis.x() = jIntern[2][0].get<double>();
                    axis.y() = jIntern[2][1].get<double>();
                    axis.z() = jIntern[2][2].get<double>();
                    axis.normalize();
                    q = q.addRotationRight(Eigen::Quaterniond(Eigen::AngleAxisd(angle, axis)));
                }
            }
        }
    };

    template<>
    struct adl_serializer<AndreiUtils::Symmetry> {
        using T = AndreiUtils::Symmetry;

        static void to_json(nlohmann::json &j, T const &data) {
            j["type"] = data.type;
            std::vector<double> v(3);
            auto symmetryAxis = data.axis.normalized();
            for (int i = 0; i < 3; i++) {
                v[i] = symmetryAxis[i];
            }
            j["axis"] = v;
            for (int i = 0; i < 3; i++) {
                v[i] = data.axisDisplacementFromOrigin[i];
            }
            j["axisDisplacementFromOrigin"] = v;
            if (data.type == "rotation") {
                std::pair<double, double> degRange(AndreiUtils::rad2Deg(data.range.first),
                                                   AndreiUtils::rad2Deg(data.range.second));
                AndreiUtils::bringValueInCircularInterval<double>(degRange.first, 0, 360);
                AndreiUtils::bringValueInCircularInterval<double>(degRange.second, 0, 360);
                j["range"] = degRange;
            } else {
                j["range"] = data.range;
            }
        }

        static void from_json(nlohmann::json const &j, T &data) {
            data.type = j.at("type").get<std::string>();
            std::vector<double> v = j.at("axis").get<std::vector<double>>();
            for (int i = 0; i < 3; i++) {
                data.axis[i] = v[i];
            }
            data.axis.normalize();
            if (j.contains("axisDisplacementFromOrigin")) {
                v = j.at("axisDisplacementFromOrigin").get<std::vector<double>>();
                for (int i = 0; i < 3; i++) {
                    data.axisDisplacementFromOrigin[i] = v[i];
                }
            }
            data.range = j.at("range").get<std::pair<double, double>>();
            if (data.type == "rotation") {
                AndreiUtils::bringValueInCircularInterval<double>(data.range.first, 0, 360);
                AndreiUtils::bringValueInCircularInterval<double>(data.range.second, 0, 360);
                data.range.first = AndreiUtils::deg2Rad(data.range.first);
                data.range.second = AndreiUtils::deg2Rad(data.range.second);
            }
        }
    };

    template<>
    struct adl_serializer<AndreiUtils::MotionDeviceIntrinsicParameters> {
        using T = AndreiUtils::MotionDeviceIntrinsicParameters;

        static void to_json(nlohmann::json &j, T const &data) {
            j.clear();
            j["data"] = data.data;
            j["biasVariances"] = data.biasVariances;
            j["noiseVariances"] = data.noiseVariances;
        }

        static void from_json(nlohmann::json const &j, T &data) {
            data.data = j.at("data").get<Eigen::MatrixXd>();
            data.biasVariances = j.at("biasVariances").get<Eigen::VectorXd>();
            data.noiseVariances = j.at("noiseVariances").get<Eigen::VectorXd>();
        }
    };

    template<>
    struct adl_serializer<AndreiUtils::MotionDeviceCaptureParameters> {
        using T = AndreiUtils::MotionDeviceCaptureParameters;

        static void to_json(nlohmann::json &j, T const &data) {
            j.clear();
            j["fps"] = data.fps;
            j["intrinsics"] = data.intrinsics;
        }

        static void from_json(nlohmann::json const &j, T &data) {
            data.fps = j.at("fps").get<double>();
            data.intrinsics = j.at("intrinsics").get<AndreiUtils::MotionDeviceIntrinsicParameters>();
        }
    };

    template<typename Type, int SpatialDimension, int SpatialDivision, int Depth>
    struct adl_serializer<AndreiUtils::SuperCube<Type, SpatialDimension, SpatialDivision, Depth>> {
        using T = AndreiUtils::SuperCube<Type, SpatialDimension, SpatialDivision, Depth>;

        static void to_json(nlohmann::json &j, T const &data) {
            // TODO: serialize other data (e.g. the min- and maxCorner and size)
            j = data.subCubes.subCubes;
        }

        static void from_json(nlohmann::json const &j, T &data) {
            // TODO: deserialize other data (e.g. the min- and maxCorner and size) and set the parent pointer correctly!
            data.subCubes.subCubes = j.get<std::map<int, typename T::SubCube>>();
        }
    };

    template<typename Type, int SpatialDimension, int SpatialDivision>
    struct adl_serializer<AndreiUtils::SuperCube<Type, SpatialDimension, SpatialDivision, 0>> {
        using T = AndreiUtils::SuperCube<Type, SpatialDimension, SpatialDivision, 0>;

        static void to_json(nlohmann::json &j, T const &data) {
            j = data.data;
        }

        static void from_json(nlohmann::json const &j, T &data) {
            data.data = j.get<typename T::Data>();
        }
    };
}
