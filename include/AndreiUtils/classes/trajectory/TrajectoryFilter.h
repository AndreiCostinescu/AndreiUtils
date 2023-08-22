//
// Created by andrei on 05.08.23.
//

#pragma once

#include <AndreiUtils/classes/trajectory/Trajectory.h>

namespace AndreiUtils {
    class TrajectoryFilter {
    public:
        TrajectoryFilter();

        virtual ~TrajectoryFilter();

        virtual Trajectory filter(Trajectory const &trajectory);

        virtual void filterInPlace(Trajectory &trajectory);

        virtual std::shared_ptr<Trajectory> filter(Trajectory const *trajectory);

        virtual void filterInPlace(Trajectory *trajectory) = 0;

        virtual std::shared_ptr<Trajectory> filter(std::shared_ptr<Trajectory> const &trajectory);

        virtual void filterInPlace(std::shared_ptr<Trajectory> &trajectory);

        virtual void reset();
    };
}
