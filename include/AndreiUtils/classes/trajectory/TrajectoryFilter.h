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

        virtual void filterInPlace(Trajectory &trajectory) = 0;
    };
}
