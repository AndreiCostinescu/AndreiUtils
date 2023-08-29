//
// Created by Andrei on 27.08.21.
//

#ifndef ANDREIUTILS_UTILSEIGENOPENCV_H
#define ANDREIUTILS_UTILSEIGENOPENCV_H

#include <AndreiUtils/classes/DualQuaternion.hpp>
#include <AndreiUtils/utilsOpenCV.h>
#include <complex>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
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
    class EigenMatrixOpenCVSerializer {
    public:
        static Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> getMatrix(cv::FileNode const &node) {
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
                                        options(), maxRows(), maxCols() {}

        explicit EigenMatrixOpenCVSerializer(const Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> &m) :
                EigenMatrixOpenCVSerializer() {
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

        void readParameters(cv::FileNode const &node) {
            typedef std::vector<Scalar> V;
            typedef Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> Mat;

            int setItems = 0;
            V data;
            for (auto item: node) {
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
                        "Poorly formatted xml data for EigenMatrixOpenCVSerializer: " + std::string(node));
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

    Eigen::Array2f cvPointToEigenArray(cv::Point2f const &point);

    Eigen::Array2d cvPointToEigenArray(cv::Point2d const &point);

    cv::Point2f eigenArrayToCVPoint(Eigen::Array2f const &array);

    cv::Point2d eigenArrayToCVPoint(Eigen::Array2d const &array);

    Eigen::Array3f cvPointToEigenArray(cv::Point3f const &point);

    Eigen::Array3d cvPointToEigenArray(cv::Point3d const &point);

    cv::Point3f eigenArrayToCVPoint(Eigen::Array3f const &array);

    cv::Point3d eigenArrayToCVPoint(Eigen::Array3d const &array);

    Eigen::Vector2f cvPointToEigenVector(cv::Point2f const &point);

    Eigen::Vector2d cvPointToEigenVector(cv::Point2d const &point);

    cv::Point2f eigenVectorToCVPoint(Eigen::Vector2f const &vector);

    cv::Point2d eigenVectorToCVPoint(Eigen::Vector2d const &vector);

    Eigen::Vector3f cvPointToEigenVector(cv::Point3f const &point);

    Eigen::Vector3d cvPointToEigenVector(cv::Point3d const &point);

    cv::Point3f eigenVectorToCVPoint(Eigen::Vector3f const &vector);

    cv::Point3d eigenVectorToCVPoint(Eigen::Vector3d const &vector);

    Eigen::Vector2f cvVectorToEigenVector(cv::Vec2f const &v);

    Eigen::Vector2d cvVectorToEigenVector(cv::Vec2d const &v);

    cv::Vec2f eigenVectorToCVVector(Eigen::Vector2f const &v);

    cv::Vec2d eigenVectorToCVVector(Eigen::Vector2d const &v);

    Eigen::Vector3f cvVectorToEigenVector(cv::Vec3f const &v);

    Eigen::Vector3d cvVectorToEigenVector(cv::Vec3d const &v);

    cv::Vec3f eigenVectorToCVVector(Eigen::Vector3f const &v);

    cv::Vec3d eigenVectorToCVVector(Eigen::Vector3d const &v);

    void writeEigenArray(cv::FileStorage &fs, Eigen::ArrayXf const &x);

    void readEigenArray(cv::FileNode const &node, Eigen::ArrayXf &x);

    void write(cv::FileStorage &fs, std::string const &name, Eigen::ArrayXf const &x);

    void read(cv::FileNode const &node, Eigen::ArrayXf &x, Eigen::ArrayXf const &default_value = Eigen::ArrayXf());

    void write(cv::FileStorage &fs, std::string const &name, Eigen::Array3f const &x);

    void read(cv::FileNode const &node, Eigen::Array3f &x,
              Eigen::Array3f const &default_value = Eigen::Array3f::Zero());

    Eigen::Matrix4d recoverMatPoseFrom2dAnd3dPoints(
            std::vector<cv::Point2f> const &eigenPoints2d, std::vector<cv::Point3f> const &eigenPoints3d,
            double fx, double fy, double ppx, double ppy, float distortionCoefficients[5]);

    Eigen::Matrix4d recoverMatPoseFrom2dAnd3dPoints(
            std::vector<Eigen::Vector2f> const &eigenPoints2d, std::vector<Eigen::Vector3f> const &eigenPoints3d,
            double fx, double fy, double ppx, double ppy, float distortionCoefficients[5]);

    Eigen::Matrix4d recoverMatPoseFrom2dAnd3dPoints(
            std::vector<cv::Point2f> const &eigenPoints2d, std::vector<cv::Point3f> const &eigenPoints3d,
            const CameraIntrinsicParameters &intrinsics);

    Eigen::Matrix4d recoverMatPoseFrom2dAnd3dPoints(
            std::vector<Eigen::Vector2f> const &eigenPoints2d, std::vector<Eigen::Vector3f> const &eigenPoints3d,
            const CameraIntrinsicParameters &intrinsics);

    DualQuaternion<double> recoverPoseFrom2dAnd3dPoints(
            std::vector<cv::Point2f> const &eigenPoints2d, std::vector<cv::Point3f> const &eigenPoints3d,
            double fx, double fy, double ppx, double ppy, float distortionCoefficients[5]);

    DualQuaternion<double> recoverPoseFrom2dAnd3dPoints(
            std::vector<Eigen::Vector2f> const &eigenPoints2d, std::vector<Eigen::Vector3f> const &eigenPoints3d,
            double fx, double fy, double ppx, double ppy, float distortionCoefficients[5]);

    DualQuaternion<double> recoverPoseFrom2dAnd3dPoints(
            std::vector<cv::Point2f> const &eigenPoints2d, std::vector<cv::Point3f> const &eigenPoints3d,
            const CameraIntrinsicParameters &intrinsics);

    DualQuaternion<double> recoverPoseFrom2dAnd3dPoints(
            std::vector<Eigen::Vector2f> const &eigenPoints2d, std::vector<Eigen::Vector3f> const &eigenPoints3d,
            const CameraIntrinsicParameters &intrinsics);
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
    void write(cv::FileStorage &fs, std::string const &name,
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
    void read(cv::FileNode const &node, Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> &x,
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
