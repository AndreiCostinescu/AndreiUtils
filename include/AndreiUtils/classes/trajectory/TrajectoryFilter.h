//
// Created by Andrei on 05.08.23.
//

#pragma once

#include <AndreiUtils/classes/trajectory/Trajectory.h>

namespace AndreiUtils {
    class TrajectoryFilter {
    public:
        TrajectoryFilter();

        virtual ~TrajectoryFilter();

        [[nodiscard]] virtual Trajectory filter(Trajectory const &trajectory);

        virtual void filterInPlace(Trajectory &trajectory);

        [[nodiscard]] virtual std::shared_ptr<Trajectory> filter(Trajectory const *trajectory);

        virtual void filterInPlace(Trajectory *trajectory) = 0;

        [[nodiscard]] virtual std::shared_ptr<Trajectory> filter(std::shared_ptr<Trajectory> const &trajectory);

        virtual void filterInPlace(std::shared_ptr<Trajectory> &trajectory);

        virtual void reset();
    };
}
