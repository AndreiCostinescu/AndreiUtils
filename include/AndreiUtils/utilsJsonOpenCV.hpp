//
// Created by andrei on 27.08.21.
//

#ifndef ANDREIUTILS_UTILSJSONOPENCV_HPP
#define ANDREIUTILS_UTILSJSONOPENCV_HPP

#include <AndreiUtils/json.hpp>
#include <opencv2/opencv.hpp>

namespace AndreiUtils {
    template<class T>
    void to_json(nlohmann::json &j, const cv::Point_<T> &p) {
        j["x"] = p.x;
        j["y"] = p.y;
    }

    template<class T>
    void from_json(const nlohmann::json &j, cv::Point_<T> &p) {
        p.x = j["x"].get<T>();
        p.y = j["y"].get<T>();
    }

    template<class T>
    void to_json(nlohmann::json &j, const cv::Point3_<T> &p) {
        j["x"] = p.x;
        j["y"] = p.y;
        j["z"] = p.z;
    }

    template<class T>
    void from_json(const nlohmann::json &j, cv::Point3_<T> &p) {
        p.x = j["x"].get<T>();
        p.y = j["y"].get<T>();
        p.z = j["z"].get<T>();
    }
}

namespace nlohmann {
    template<class T>
    struct adl_serializer<cv::Point_<T>> {
        static void to_json(nlohmann::json &j, const cv::Point_<T> &p) {
            /*
            j["x"] = p.x;
            j["y"] = p.y;
            /*/
            AndreiUtils::to_json(j, p);
            //*/
        }

        static void from_json(const nlohmann::json &j, cv::Point_<T> &p) {
            /*
            p.x = j["x"].get<T>();
            p.y = j["y"].get<T>();
            /*/
            AndreiUtils::from_json(j, p);
            //*/
        }
    };

    template<class T>
    struct adl_serializer<cv::Point3_<T>> {
        static void to_json(nlohmann::json &j, const cv::Point3_<T> &p) {
            /*
            j["x"] = p.x;
            j["y"] = p.y;
            j["z"] = p.z;
            /*/
            AndreiUtils::to_json(j, p);
            //*/
        }

        static void from_json(const nlohmann::json &j, cv::Point3_<T> &p) {
            /*
            p.x = j["x"].get<T>();
            p.y = j["y"].get<T>();
            p.z = j["z"].get<T>();
            /*/
            AndreiUtils::from_json(j, p);
            //*/
        }
    };
}

#endif //ANDREIUTILS_UTILSJSONOPENCV_HPP
