//
// Created by Andrei on 27.08.21.
//

#ifndef ANDREIUTILS_UTILSEIGENOPENCV_H
#define ANDREIUTILS_UTILSEIGENOPENCV_H

#include <AndreiUtils/utilsOpenCV.h>
#include <complex>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>

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
    class EigenMatrixOpenCVSerializer {
    public:
        static Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> getMatrix(const cv::FileNode &node) {
            EigenMatrixOpenCVSerializer serializer;
            serializer.readParameters(node);
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

        EigenMatrixOpenCVSerializer() : matrix(nullptr), type(), rows(), cols(), currentRows(), currentCols(),
                                        options(),
                                        maxRows(), maxCols() {}

        explicit EigenMatrixOpenCVSerializer(const Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> &m)
                : EigenMatrixOpenCVSerializer() {
            this->matrix = &m;
            this->type = EigenMatrixOpenCVSerializer::getStringFromType();
            this->rows = Rows;
            this->cols = Cols;
            this->currentRows = m.rows();
            this->currentCols = m.cols();
            this->options = Options;
            this->maxRows = MaxRows;
            this->maxCols = MaxCols;
        }

        void writeParameters(cv::FileStorage &fs) const {
            fs << "{";
            fs << "type" << this->type;
            fs << "rows" << this->rows;
            fs << "cols" << this->cols;
            fs << "curRows" << this->currentRows;
            fs << "curCols" << this->currentCols;
            fs << "options" << this->options;
            fs << "maxRows" << this->maxRows;
            fs << "maxCols" << this->maxCols;

            typedef std::vector<Scalar> V;
            typedef Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> Mat;
            V data(this->currentRows * this->currentCols);
            for (int r = 0; r < this->currentRows; r++) {
                for (int c = 0; c < this->currentCols; c++) {
                    data[r * this->currentCols + c] = (*((Mat *) matrix))(r, c);
                }
            }
            fs << "data" << data;
            fs << "}";
        }

        void readParameters(const cv::FileNode &node) {
            typedef std::vector<Scalar> V;
            typedef Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> Mat;

            int setItems = 0;
            V data;
            for (auto item : node) {
                std::string name = item.name();
                if (name == "type") {
                    this->type = (std::string) item;
                    if (EigenMatrixOpenCVSerializer::getStringFromType() != this->type) {
                        throw std::runtime_error("Requested matrix type is different than the stored matrix type");
                    }
                    setItems++;
                } else if (name == "options") {
                    this->options = (int) item;
                    if (Options != this->options) {
                        throw std::runtime_error(
                                "Requested matrix options is different than the stored matrix options");
                    }
                    setItems++;
                } else if (name == "maxRows") {
                    this->maxRows = (int) item;
                    if (MaxRows != -1 && this->maxRows != -1 && MaxRows != this->maxRows) {
                        throw std::runtime_error(
                                "Requested matrix max rows is different than the stored matrix max rows");
                    }
                    setItems++;
                } else if (name == "maxCols") {
                    this->maxCols = (int) item;
                    if (MaxCols != -1 && this->maxCols != -1 && MaxCols != this->maxCols) {
                        throw std::runtime_error(
                                "Requested matrix max columns is different than the stored matrix max columns");
                    }
                    setItems++;
                } else if (name == "data") {
                    // data = (V) item;
                    cv::read(node["data"], data);
                    // data = (std::vector<double>) node["data"];
                    setItems++;
                } else if (name == "rows") {
                    this->rows = (int) item;
                    setItems++;
                } else if (name == "cols") {
                    this->cols = (int) item;
                    setItems++;
                } else if (name == "curRows") {
                    this->currentRows = (int) item;
                    setItems++;
                } else if (name == "curCols") {
                    this->currentCols = (int) item;
                    setItems++;
                }
            }
            if (setItems != 9) {
                throw std::runtime_error(
                        "Poorly formatted xml data for EigenMatrixJsonSerializer: " + std::string(node));
            }
            if (Rows != -1) {
                if (this->rows != -1 && Rows != this->rows) {
                    throw std::runtime_error("Requested matrix rows is different than the stored matrix rows");
                } else if (this->rows == -1 && Rows != this->currentRows) {
                    throw std::runtime_error("Requested matrix rows is different than the current matrix rows");
                }
            }
            if (Cols != -1) {
                if (this->cols != -1 && Cols != this->cols) {
                    throw std::runtime_error("Requested matrix columns is different than the stored matrix columns");
                } else if (this->cols == -1 && Cols != this->currentCols) {
                    throw std::runtime_error("Requested matrix columns is different than the current matrix columns");
                }
            }

            this->matrix = new Mat();
            *((Mat *) this->matrix) = Mat::Zero(this->currentRows, this->currentCols);
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

    Eigen::Array3f cvPointToEigenArray(const cv::Point3f &point);

    cv::Point3f eigenArrayToCVPoint(const Eigen::Array3f &array);

    void writeEigenArray(cv::FileStorage &fs, const Eigen::ArrayXf &x);

    void readEigenArray(const cv::FileNode &node, Eigen::ArrayXf &x);

    void write(cv::FileStorage &fs, const std::string &name, const Eigen::ArrayXf &x);

    void read(const cv::FileNode &node, Eigen::ArrayXf &x, const Eigen::ArrayXf &default_value = Eigen::ArrayXf());

    void write(cv::FileStorage &fs, const std::string &name, const Eigen::Array3f &x);

    void read(const cv::FileNode &node, Eigen::Array3f &x,
              const Eigen::Array3f &default_value = Eigen::Array3f::Zero());
}

namespace cv {
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
    void write(cv::FileStorage &fs, const std::string &name,
               const Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> &x) {
        AndreiUtils::EigenMatrixOpenCVSerializer<Scalar, Rows, Cols, Options, MaxRows, MaxCols>(x).writeParameters(fs);
    }

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
    void read(const cv::FileNode &node, Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> &x,
              const Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> &default_value = Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>()) {
        if (node.empty()) {
            x = default_value;
        } else {
            x = AndreiUtils::EigenMatrixOpenCVSerializer<Scalar, Rows, Cols, Options, MaxRows, MaxCols>::getMatrix(
                    node);
        }
    }
}

#endif //ANDREIUTILS_UTILSEIGENOPENCV_H
