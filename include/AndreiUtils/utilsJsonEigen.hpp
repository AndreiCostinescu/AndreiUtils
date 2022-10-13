//
// Created by Andrei on 27.08.21.
//

#ifndef ANDREIUTILS_UTILSJSONEIGEN_HPP
#define ANDREIUTILS_UTILSJSONEIGEN_HPP

#include <AndreiUtils/classes/DualQuaternion.hpp>
#include <AndreiUtils/classes/Symmetry.h>
#include <AndreiUtils/classes/motion/MotionDeviceCaptureParameters.h>
#include <AndreiUtils/classes/motion/MotionDeviceIntrinsicParameters.h>
#include <AndreiUtils/json.hpp>
#include <AndreiUtils/utilsJson.h>
#include <complex>
#include <Eigen/Dense>
#include <string>
#include <vector>

namespace AndreiUtils {
    template<typename Scalar, int Rows, int Cols,
            int Options = Eigen::AutoAlign |
                          #if EIGEN_GNUC_AT(3, 4)
                          // workaround a bug in at least gcc 3.4.6
                          // the innermost ?: ternary operator is misparsed. We write it slightly
                          // differently and this makes gcc 3.4.6 happy, but it's ugly.
                          // The error would only show up with EIGEN_DEFAULT_TO_ROW_MAJOR is defined
                          // (when EIGEN_DEFAULT_MATRIX_STORAGE_ORDER_OPTION is RowMajor)
                          ((Rows==1 && Cols!=1) ? Eigen::RowMajor
                          : !(Cols==1 && Rows!=1) ? EIGEN_DEFAULT_MATRIX_STORAGE_ORDER_OPTION
                          : Eigen::ColMajor ),
                          #else
                          ((Rows == 1 && Cols != 1) ? Eigen::RowMajor :
                           ((Cols == 1 && Rows != 1) ? Eigen::ColMajor :
                            EIGEN_DEFAULT_MATRIX_STORAGE_ORDER_OPTION)),
#endif
            int MaxRows = Rows,
            int MaxCols = Cols>
    class EigenMatrixJsonSerializer {
    public:
        static Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> getMatrix(const nlohmann::json &j) {
            EigenMatrixJsonSerializer serializer;
            serializer.fromJson(j);
            return serializer.getMatrix();
        };

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

        EigenMatrixJsonSerializer() : matrix(nullptr), type(), rows(), cols(), currentRows(), currentCols(), options(),
                                      maxRows(), maxCols() {}

        explicit EigenMatrixJsonSerializer(const Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> &m)
                : EigenMatrixJsonSerializer() {
            this->matrix = &m;
            this->type = EigenMatrixJsonSerializer::getStringFromType();
            this->rows = Rows;
            this->cols = Cols;
            this->currentRows = m.rows();
            this->currentCols = m.cols();
            this->options = Options;
            this->maxRows = MaxRows;
            this->maxCols = MaxCols;
        }

        void toJson(nlohmann::json &j) const {
            j["type"] = this->type;
            j["rows"] = this->rows;
            j["cols"] = this->cols;
            j["curRows"] = this->currentRows;
            j["curCols"] = this->currentCols;
            j["options"] = this->options;
            j["maxRows"] = this->maxRows;
            j["maxCols"] = this->maxCols;

            typedef std::vector<Scalar> V;
            typedef Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> Mat;
            V data(this->currentRows * this->currentCols);
            for (int r = 0; r < this->currentRows; r++) {
                for (int c = 0; c < this->currentCols; c++) {
                    data[r * this->currentCols + c] = (*((Mat *) matrix))(r, c);
                }
            }
            j["data"] = data;
        }

        void fromJson(const nlohmann::json &j) {
            if (!(j.template contains("type") && j.template contains("rows") && j.template contains("cols") &&
                  j.template contains("curRows") && j.template contains("curCols") && j.template contains("options") &&
                  j.template contains("maxRows") && j.template contains("maxCols") && j.template contains("data"))) {
                throw std::runtime_error("Poorly formatted json data for EigenMatrixJsonSerializer: " + j.dump());
            }
            this->type = j["type"].get<std::string>();
            if (EigenMatrixJsonSerializer::getStringFromType() != this->type) {
                throw std::runtime_error("Requested matrix type is different than the stored matrix type");
            }
            this->rows = j["rows"].get<int>();
            this->currentRows = j["curRows"].get<int>();
            if (Rows != -1) {
                if (this->rows != -1 && Rows != this->rows) {
                    throw std::runtime_error("Requested matrix rows is different than the stored matrix rows");
                } else if (this->rows == -1 && Rows != this->currentRows) {
                    throw std::runtime_error("Requested matrix rows is different than the current matrix rows");
                }
            }
            this->cols = j["cols"].get<int>();
            this->currentCols = j["curCols"].get<int>();
            if (Cols != -1) {
                if (this->cols != -1 && Cols != this->cols) {
                    throw std::runtime_error("Requested matrix columns is different than the stored matrix columns");
                } else if (this->cols == -1 && Cols != this->currentCols) {
                    throw std::runtime_error("Requested matrix columns is different than the current matrix columns");
                }
            }
            this->options = j["options"].get<int>();
            if (Options != this->options) {
                throw std::runtime_error("Requested matrix options is different than the stored matrix options");
            }
            this->maxRows = j["maxRows"].get<int>();
            if (MaxRows != -1 && this->maxRows != -1 && MaxRows != this->maxRows) {
                throw std::runtime_error("Requested matrix max rows is different than the stored matrix max rows");
            }
            this->maxCols = j["maxCols"].get<int>();
            if (MaxCols != -1 && this->maxCols != -1 && MaxCols != this->maxCols) {
                throw std::runtime_error(
                        "Requested matrix max columns is different than the stored matrix max columns");
            }

            typedef std::vector<Scalar> V;
            typedef Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> Mat;
            this->matrix = new Mat();
            *((Mat *) this->matrix) = Mat::Zero(this->currentRows, this->currentCols);
            V data = j["data"].get<V>();
            for (int r = 0; r < this->currentRows; r++) {
                for (int c = 0; c < this->currentCols; c++) {
                    (*((Mat *) matrix))(r, c) = data[r * this->currentCols + c];
                }
            }
        }

        Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> getMatrix() const {
            if (this->matrix == nullptr) {
                throw std::runtime_error("Matrix is void!");
            }
            typedef Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> Mat;
            return *((Mat *) this->matrix);
        }

    private:
        const void *matrix;
        std::string type;
        int rows, cols, options, maxRows, maxCols, currentRows, currentCols;
    };

    class SymmetryJsonSerializer {
    public:
        SymmetryJsonSerializer(Symmetry const &s);

        SymmetryJsonSerializer(Symmetry &s);

        void toJson(nlohmann::json &j) const;

        void fromJson(nlohmann::json const &j);

    private:
        Symmetry const *sConst;
        Symmetry *s;
    };

    class MotionDeviceIntrinsicParametersJsonSerializer {
    public:
        MotionDeviceIntrinsicParametersJsonSerializer(MotionDeviceIntrinsicParameters const &m);

        MotionDeviceIntrinsicParametersJsonSerializer(MotionDeviceIntrinsicParameters &m);

        void toJson(nlohmann::json &j) const;

        void fromJson(nlohmann::json const &j);

    private:
        MotionDeviceIntrinsicParameters const *mConst;
        MotionDeviceIntrinsicParameters *m;
    };

    class MotionDeviceCaptureParametersJsonSerializer {
    public:
        MotionDeviceCaptureParametersJsonSerializer(MotionDeviceCaptureParameters const &m);

        MotionDeviceCaptureParametersJsonSerializer(MotionDeviceCaptureParameters &m);

        void toJson(nlohmann::json &j) const;

        void fromJson(nlohmann::json const &j);

    private:
        MotionDeviceCaptureParameters const *mConst;
        MotionDeviceCaptureParameters *m;
    };
}

namespace nlohmann {
    template<class T>
    struct adl_serializer<Eigen::Quaternion<T>> {
        static void to_json(nlohmann::json &j, const Eigen::Quaternion<T> &q) {
            std::vector<T> coefficients{q.w(), q.x(), q.y(), q.z()};
            j = coefficients;
        }

        static void from_json(const nlohmann::json &j, Eigen::Quaternion<T> &q) {
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
        static void to_json(nlohmann::json &j, const AndreiUtils::DualQuaternion<T> &q) {
            j = q.coefficients();
        }

        static void from_json(const nlohmann::json &j, AndreiUtils::DualQuaternion<T> &q) {
            q.fromCoefficients(j.get<std::vector<T>>());
        }
    };

    template<>
    struct adl_serializer<AndreiUtils::Symmetry> {
        static void to_json(nlohmann::json &j, const AndreiUtils::Symmetry &s) {
            AndreiUtils::SymmetryJsonSerializer(s).toJson(j);
        }

        static void from_json(const nlohmann::json &j, AndreiUtils::Symmetry &s) {
            AndreiUtils::SymmetryJsonSerializer(s).fromJson(j);
        }
    };

    template<>
    struct adl_serializer<AndreiUtils::MotionDeviceIntrinsicParameters> {
        static void to_json(nlohmann::json &j, const AndreiUtils::MotionDeviceIntrinsicParameters &p) {
            AndreiUtils::MotionDeviceIntrinsicParametersJsonSerializer(p).toJson(j);
        }

        static void from_json(const nlohmann::json &j, AndreiUtils::MotionDeviceIntrinsicParameters &p) {
            AndreiUtils::MotionDeviceIntrinsicParametersJsonSerializer(p).fromJson(j);
        }
    };

    template<>
    struct adl_serializer<AndreiUtils::MotionDeviceCaptureParameters> {
        static void to_json(nlohmann::json &j, const AndreiUtils::MotionDeviceCaptureParameters &p) {
            AndreiUtils::MotionDeviceCaptureParametersJsonSerializer(p).toJson(j);
        }

        static void from_json(const nlohmann::json &j, AndreiUtils::MotionDeviceCaptureParameters &p) {
            AndreiUtils::MotionDeviceCaptureParametersJsonSerializer(p).fromJson(j);
        }
    };

    template<class T>
    struct adl_serializer<Eigen::Matrix<T, 2, 1>> {
        static void to_json(nlohmann::json &j, const Eigen::Matrix<T, 2, 1> &v) {
            j["x"] = v(0);
            j["y"] = v(1);
        }

        static void from_json(const nlohmann::json &j, Eigen::Matrix<T, 2, 1> &v) {
            v(0) = j["x"].get<T>();
            v(1) = j["y"].get<T>();
        }
    };

    template<class T>
    struct adl_serializer<Eigen::Matrix<T, 3, 1>> {
        static void to_json(nlohmann::json &j, const Eigen::Matrix<T, 3, 1> &v) {
            j["x"] = v(0);
            j["y"] = v(1);
            j["z"] = v(2);
        }

        static void from_json(const nlohmann::json &j, Eigen::Matrix<T, 3, 1> &v) {
            v(0) = j["x"].get<T>();
            v(1) = j["y"].get<T>();
            v(2) = j["z"].get<T>();
        }
    };

    template<typename Scalar, int Rows, int Cols>
    struct adl_serializer<Eigen::Matrix<Scalar, Rows, Cols>> {
        static void to_json(nlohmann::json &j,
                            const Eigen::Matrix<Scalar, Rows, Cols,
                                    Eigen::AutoAlign |
                                    #if EIGEN_GNUC_AT(3, 4)
                                    // workaround a bug in at least gcc 3.4.6
                                    // the innermost ?: ternary operator is misparsed. We write it slightly
                                    // differently and this makes gcc 3.4.6 happy, but it's ugly.
                                    // The error would only show up with EIGEN_DEFAULT_TO_ROW_MAJOR is defined
                                    // (when EIGEN_DEFAULT_MATRIX_STORAGE_ORDER_OPTION is RowMajor)
                                    ((Rows==1 && Cols!=1) ? Eigen::RowMajor
                                    : !(Cols==1 && Rows!=1) ? EIGEN_DEFAULT_MATRIX_STORAGE_ORDER_OPTION
                                    : Eigen::ColMajor ),
                                    #else
                                    ((Rows == 1 && Cols != 1) ? Eigen::RowMajor :
                                     ((Cols == 1 && Rows != 1) ? Eigen::ColMajor :
                                      EIGEN_DEFAULT_MATRIX_STORAGE_ORDER_OPTION)),
#endif
                                    Rows, Cols> &m) {
            AndreiUtils::EigenMatrixJsonSerializer<Scalar, Rows, Cols,
                    Eigen::AutoAlign |
                    #if EIGEN_GNUC_AT(3, 4)
                    // workaround a bug in at least gcc 3.4.6
                                    // the innermost ?: ternary operator is misparsed. We write it slightly
                                    // differently and this makes gcc 3.4.6 happy, but it's ugly.
                                    // The error would only show up with EIGEN_DEFAULT_TO_ROW_MAJOR is defined
                                    // (when EIGEN_DEFAULT_MATRIX_STORAGE_ORDER_OPTION is RowMajor)
                                    ((Rows==1 && Cols!=1) ? Eigen::RowMajor
                                    : !(Cols==1 && Rows!=1) ? EIGEN_DEFAULT_MATRIX_STORAGE_ORDER_OPTION
                                    : Eigen::ColMajor ),
                    #else
                    ((Rows == 1 && Cols != 1) ? Eigen::RowMajor :
                     ((Cols == 1 && Rows != 1) ? Eigen::ColMajor :
                      EIGEN_DEFAULT_MATRIX_STORAGE_ORDER_OPTION)),
#endif
                    Rows, Cols> serializer(m);
            serializer.toJson(j);
        }

        static void
        from_json(const nlohmann::json &j,
                  Eigen::Matrix<Scalar, Rows, Cols,
                          Eigen::AutoAlign |
                          #if EIGEN_GNUC_AT(3, 4)
                          // workaround a bug in at least gcc 3.4.6
                                    // the innermost ?: ternary operator is misparsed. We write it slightly
                                    // differently and this makes gcc 3.4.6 happy, but it's ugly.
                                    // The error would only show up with EIGEN_DEFAULT_TO_ROW_MAJOR is defined
                                    // (when EIGEN_DEFAULT_MATRIX_STORAGE_ORDER_OPTION is RowMajor)
                                    ((Rows==1 && Cols!=1) ? Eigen::RowMajor
                                    : !(Cols==1 && Rows!=1) ? EIGEN_DEFAULT_MATRIX_STORAGE_ORDER_OPTION
                                    : Eigen::ColMajor ),
                          #else
                          ((Rows == 1 && Cols != 1) ? Eigen::RowMajor :
                           ((Cols == 1 && Rows != 1) ? Eigen::ColMajor :
                            EIGEN_DEFAULT_MATRIX_STORAGE_ORDER_OPTION)),
#endif
                          Rows, Cols> &v) {
            v = AndreiUtils::EigenMatrixJsonSerializer<Scalar, Rows, Cols,
                    Eigen::AutoAlign |
                    #if EIGEN_GNUC_AT(3, 4)
                    // workaround a bug in at least gcc 3.4.6
                                    // the innermost ?: ternary operator is misparsed. We write it slightly
                                    // differently and this makes gcc 3.4.6 happy, but it's ugly.
                                    // The error would only show up with EIGEN_DEFAULT_TO_ROW_MAJOR is defined
                                    // (when EIGEN_DEFAULT_MATRIX_STORAGE_ORDER_OPTION is RowMajor)
                                    ((Rows==1 && Cols!=1) ? Eigen::RowMajor
                                    : !(Cols==1 && Rows!=1) ? EIGEN_DEFAULT_MATRIX_STORAGE_ORDER_OPTION
                                    : Eigen::ColMajor ),
                    #else
                    ((Rows == 1 && Cols != 1) ? Eigen::RowMajor :
                     ((Cols == 1 && Rows != 1) ? Eigen::ColMajor :
                      EIGEN_DEFAULT_MATRIX_STORAGE_ORDER_OPTION)),
#endif
                    Rows, Cols>::getMatrix(j);
        }
    };

    template<typename Scalar, int Rows, int Cols, int Options>
    struct adl_serializer<Eigen::Matrix<Scalar, Rows, Cols, Options>> {
        static void to_json(nlohmann::json &j, const Eigen::Matrix<Scalar, Rows, Cols, Options, Rows, Cols> &m) {
            AndreiUtils::EigenMatrixJsonSerializer<Scalar, Rows, Cols, Options, Rows, Cols> serializer(m);
            serializer.toJson(j);
        }

        static void
        from_json(const nlohmann::json &j, Eigen::Matrix<Scalar, Rows, Cols, Options, Rows, Cols> &v) {
            v = AndreiUtils::EigenMatrixJsonSerializer<Scalar, Rows, Cols, Options, Rows, Cols>::getMatrix(j);
        }
    };

    template<typename Scalar, int Rows, int Cols, int Options, int MaxRows>
    struct adl_serializer<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows>> {
        static void to_json(nlohmann::json &j, const Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, Cols> &m) {
            AndreiUtils::EigenMatrixJsonSerializer<Scalar, Rows, Cols, Options, MaxRows, Cols> serializer(m);
            serializer.toJson(j);
        }

        static void
        from_json(const nlohmann::json &j, Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, Cols> &v) {
            v = AndreiUtils::EigenMatrixJsonSerializer<Scalar, Rows, Cols, Options, MaxRows, Cols>::getMatrix(j);
        }
    };

    template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    struct adl_serializer<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
        static void to_json(nlohmann::json &j, const Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> &m) {
            AndreiUtils::EigenMatrixJsonSerializer<Scalar, Rows, Cols, Options, MaxRows, MaxCols> serializer(m);
            serializer.toJson(j);
        }

        static void
        from_json(const nlohmann::json &j, Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> &v) {
            v = AndreiUtils::EigenMatrixJsonSerializer<Scalar, Rows, Cols, Options, MaxRows, MaxCols>::getMatrix(j);
        }
    };
}

#endif //ANDREIUTILS_UTILSJSONEIGEN_HPP
