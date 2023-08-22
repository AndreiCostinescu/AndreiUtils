//
// Created by andrei on 05.08.23.
//

#include <AndreiUtils/classes/trajectory/PoseDecoupledLowPassTrajectoryFilter.h>

using namespace AndreiUtils;
using namespace Eigen;
using namespace std;

PoseDecoupledLowPassTrajectoryFilter::PoseDecoupledLowPassTrajectoryFilter(
        int pWindowSize, double qRange, double qCenter) :
        pFilter(make_shared<SlidingWindow<Vector3d>>(pWindowSize)),
        qFilter(make_shared<QuaternionLowPassFilter<double>>(qRange, qCenter)),
        filterPosition(true), filterOrientation(true) {}

PoseDecoupledLowPassTrajectoryFilter::PoseDecoupledLowPassTrajectoryFilter(int pWindowSize) :
        pFilter(make_shared<SlidingWindow<Vector3d>>(pWindowSize)), qFilter(), filterPosition(true),
        filterOrientation(false) {}

PoseDecoupledLowPassTrajectoryFilter::PoseDecoupledLowPassTrajectoryFilter(double qRange, double qCenter) :
        pFilter(), qFilter(make_shared<QuaternionLowPassFilter<double>>(qRange, qCenter)), filterPosition(false),
        filterOrientation(true) {}

void PoseDecoupledLowPassTrajectoryFilter::filterInPlace(Trajectory *trajectory) {
    int index = 0;
    auto &poses = trajectory->getPoses();
    for (auto const &q: poses) {
        auto pos = q.getTranslation();
        auto ori = q.getRotation();
        if (this->filterPosition) {
            if (index == 0) {
                this->pFilter->clear();
            }
            this->pFilter->addCopyData(pos);
            if (this->pFilter->full()) {
                pos = this->pFilter->getAverage();
            }
        }
        if (this->filterOrientation) {
            if (index == 0) {
                this->qFilter->resetFilterValue(ori);
            } else {
                this->qFilter->filter(ori);
                ori = this->qFilter->getFilterValue();
            }
        }
        poses[index++] = Posed(ori, pos);
    }
}
