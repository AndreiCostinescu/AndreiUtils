//
// Created by andrei on 13.11.20.
//

#include <AndreiUtils/utilsCubemos.h>

using namespace std;

string AndreiUtils::default_log_dir() {
    string cubemosLogDir;
#ifdef _WINDOWS
    cubemosLogDir = string(getenv("LOCALAPPDATA")) + "\\Cubemos\\SkeletonTracking\\logs";
#elif __linux__
    cubemosLogDir = string(getenv("HOME")) + "/.cubemos/skeleton_tracking/logs";
#endif
    return cubemosLogDir;
}

string AndreiUtils::default_license_dir() {
    string cubemosLicenseDir;
#ifdef _WINDOWS
    cubemosLicenseDir = string(getenv("LOCALAPPDATA")) + "\\Cubemos\\SkeletonTracking\\license";
#elif __linux__
    cubemosLicenseDir = string(getenv("HOME")) + "/.cubemos/skeleton_tracking/license";
#endif
    return cubemosLicenseDir;
}

string AndreiUtils::default_model_dir() {
    string cubemosModelDir;
#ifdef _WINDOWS
    cubemosModelDir = string(getenv("LOCALAPPDATA")) + "\\Cubemos\\SkeletonTracking\\models";
#elif __linux__
    cubemosModelDir = string(getenv("HOME")) + "/.cubemos/skeleton_tracking/models";
#endif
    return cubemosModelDir;
}

string AndreiUtils::default_res_dir() {
    string cubemosResDir;
#ifdef _WINDOWS
    cubemosResDir = string(getenv("LOCALAPPDATA")) + "\\Cubemos\\SkeletonTracking\\res";
#elif __linux__
    cubemosResDir = string(getenv("HOME")) + "/.cubemos/skeleton_tracking/res";
#endif
    return cubemosResDir;
}
