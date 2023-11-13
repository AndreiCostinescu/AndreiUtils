//
// Created by Andrei on 05.08.23.
//

#pragma once

#include <AndreiUtils/classes/trajectory/CartesianTrajectory.h>

namespace AndreiUtils {
    class CartesianTrajectoryFilter {
    public:
        CartesianTrajectoryFilter();

        virtual ~CartesianTrajectoryFilter();

        [[nodiscard]] virtual CartesianTrajectory filter(CartesianTrajectory const &trajectory);

        virtual void filterInPlace(CartesianTrajectory &trajectory);

        [[nodiscard]] virtual std::shared_ptr<CartesianTrajectory> filter(CartesianTrajectory const *trajectory);

        virtual void filterInPlace(CartesianTrajectory *trajectory) = 0;

        [[nodiscard]] virtual std::shared_ptr<CartesianTrajectory> filter(
                std::shared_ptr<CartesianTrajectory> const &trajectory);

        virtual void filterInPlace(std::shared_ptr<CartesianTrajectory> &trajectory);

        virtual void reset();
    };
}
