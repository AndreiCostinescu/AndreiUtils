//
// Created by andrei on 06.09.21.
//

#ifndef ANDREIUTILS_UTILSEIGEN_HPP
#define ANDREIUTILS_UTILSEIGEN_HPP

#include <Eigen/Dense>

namespace AndreiUtils {
    // https://gist.github.com/javidcf/25066cf85e71105d57b6
    template<class MatT>
    Eigen::Matrix<typename MatT::Scalar, MatT::ColsAtCompileTime, MatT::RowsAtCompileTime> pseudoinverse(
            const MatT &mat, typename MatT::Scalar tolerance = typename MatT::Scalar{1e-4}) {
        typedef typename MatT::Scalar Scalar;
        auto svd = mat.jacobiSvd(Eigen::ComputeFullU | Eigen::ComputeFullV);
        const auto &singularValues = svd.singularValues();
        Eigen::Matrix<Scalar, MatT::ColsAtCompileTime, MatT::RowsAtCompileTime> singularValuesInv(mat.cols(),
                                                                                                  mat.rows());
        singularValuesInv.setZero();
        for (unsigned int i = 0; i < singularValues.size(); ++i) {
            if (singularValues(i) > tolerance) {
                singularValuesInv(i, i) = Scalar{1} / singularValues(i);
            } else {
                singularValuesInv(i, i) = Scalar{0};
            }
        }
        return svd.matrixV() * singularValuesInv * svd.matrixU().adjoint();
    }
}

#endif //ANDREIUTILS_UTILSEIGEN_HPP
