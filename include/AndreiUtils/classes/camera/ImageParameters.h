//
// Created by Andrei on 21-Jan-22.
//

#ifndef ANDREIUTILS_IMAGEPARAMETERS_H
#define ANDREIUTILS_IMAGEPARAMETERS_H

#include <AndreiUtils/json.hpp>

namespace AndreiUtils {
    class ImageParameters {
    public:
        ImageParameters();

        ImageParameters(int height, int width);

        virtual ~ImageParameters();

        void setImageParameters(int _h, int _w);

        void toJson(nlohmann::json &j) const;

        void fromJson(const nlohmann::json &j);

        int h, w;
    };
}

namespace nlohmann {
    template<> struct adl_serializer<AndreiUtils::ImageParameters> {
        static void to_json(nlohmann::json& j, const AndreiUtils::ImageParameters& p) {
            p.toJson(j);
        }

        static void from_json(const nlohmann::json& j, AndreiUtils::ImageParameters& p) {
            p.fromJson(j);
        }
    };
}

#endif //ANDREIUTILS_IMAGEPARAMETERS_H
