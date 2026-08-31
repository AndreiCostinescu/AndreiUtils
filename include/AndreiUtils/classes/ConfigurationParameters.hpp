// Copyright 2026 AndreiUtils Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//
// Created by Andrei on 10.11.22.
//

#pragma once

#include <AndreiUtils/classes/Parameters.hpp>

namespace AndreiUtils {
    class ConfigurationParameters : public Parameters {
    public:
        static std::string const parameterForKey;

        explicit ConfigurationParameters(std::string const &fileName, std::string parametersFor);

        explicit ConfigurationParameters(nlohmann::json config, std::string parametersFor);

        ConfigurationParameters() noexcept;

        ConfigurationParameters(ConfigurationParameters const &other);

        ConfigurationParameters(ConfigurationParameters &&other) noexcept;

        ConfigurationParameters &operator=(ConfigurationParameters const &other);

        ConfigurationParameters &operator=(ConfigurationParameters &&other) noexcept;

        bool operator==(ConfigurationParameters const &other) const;

        bool operator!=(ConfigurationParameters const &other) const;

        [[nodiscard]] ConfigurationParameters getSubConfig(std::string const &subParametersFor) const;

        void clear() override;

    protected:
        bool checkCorrectJsonParameters(nlohmann::json &_config) const;

        std::string parametersFor;
    };
} // namespace AndreiUtils
