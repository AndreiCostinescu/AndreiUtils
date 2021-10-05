//
// Created by Andrei on 27.08.21.
//

#ifndef ANDREIUTILS_UTILSJSONOPENCV_HPP
#define ANDREIUTILS_UTILSJSONOPENCV_HPP

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
    class EigenMatrixSerializer {
    public:
        static Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> getMatrix(const nlohmann::json &j) {
            EigenMatrixSerializer serializer;
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

        EigenMatrixSerializer() : matrix(nullptr), type(), rows(), cols(), currentRows(), currentCols(), options(),
                                  maxRows(), maxCols() {}

        explicit EigenMatrixSerializer(const Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> &m)
                : EigenMatrixSerializer() {
            this->matrix = &m;
            this->type = EigenMatrixSerializer::getStringFromType();
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
                  j.template contains("maxRows") && j.template contains("maxCols"))) {
                throw std::runtime_error("Poorly formatted json data for EigenMatrixSerializer: " + j.dump());
            }
            this->type = j["type"].get<std::string>();
            if (EigenMatrixSerializer::getStringFromType() != this->type) {
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
}

namespace nlohmann {
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
            AndreiUtils::EigenMatrixSerializer<Scalar, Rows, Cols,
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
            v = AndreiUtils::EigenMatrixSerializer<Scalar, Rows, Cols,
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
            AndreiUtils::EigenMatrixSerializer<Scalar, Rows, Cols, Options, Rows, Cols> serializer(m);
            serializer.toJson(j);
        }

        static void
        from_json(const nlohmann::json &j, Eigen::Matrix<Scalar, Rows, Cols, Options, Rows, Cols> &v) {
            v = AndreiUtils::EigenMatrixSerializer<Scalar, Rows, Cols, Options, Rows, Cols>::getMatrix(j);
        }
    };

    template<typename Scalar, int Rows, int Cols, int Options, int MaxRows>
    struct adl_serializer<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows>> {
        static void to_json(nlohmann::json &j, const Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, Cols> &m) {
            AndreiUtils::EigenMatrixSerializer<Scalar, Rows, Cols, Options, MaxRows, Cols> serializer(m);
            serializer.toJson(j);
        }

        static void
        from_json(const nlohmann::json &j, Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, Cols> &v) {
            v = AndreiUtils::EigenMatrixSerializer<Scalar, Rows, Cols, Options, MaxRows, Cols>::getMatrix(j);
        }
    };

    template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    struct adl_serializer<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
        static void to_json(nlohmann::json &j, const Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> &m) {
            AndreiUtils::EigenMatrixSerializer<Scalar, Rows, Cols, Options, MaxRows, MaxCols> serializer(m);
            serializer.toJson(j);
        }

        static void
        from_json(const nlohmann::json &j, Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> &v) {
            v = AndreiUtils::EigenMatrixSerializer<Scalar, Rows, Cols, Options, MaxRows, MaxCols>::getMatrix(j);
        }
    };
}

#endif //ANDREIUTILS_UTILSJSONOPENCV_HPP
