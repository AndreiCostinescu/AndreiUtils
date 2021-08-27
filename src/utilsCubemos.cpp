//
// Created by andrei on 13.11.20.
//

#include <andrei_utils/utilsCubemos.h>

using namespace std;

string default_log_dir() {
    string cubemosLogDir;
#ifdef _WINDOWS
    cubemosLogDir = string(getenv("LOCALAPPDATA")) + "\\Cubemos\\SkeletonTracking\\logs";
#elif __linux__
    cubemosLogDir = string(getenv("HOME")) + "/.cubemos/skeleton_tracking/logs";
#endif
    return cubemosLogDir;
}

string default_license_dir() {
    string cubemosLicenseDir;
#ifdef _WINDOWS
    cubemosLicenseDir = string(getenv("LOCALAPPDATA")) + "\\Cubemos\\SkeletonTracking\\license";
#elif __linux__
    cubemosLicenseDir = string(getenv("HOME")) + "/.cubemos/skeleton_tracking/license";
#endif
    return cubemosLicenseDir;
}

string default_model_dir() {
    string cubemosModelDir;
#ifdef _WINDOWS
    cubemosModelDir = string(getenv("LOCALAPPDATA")) + "\\Cubemos\\SkeletonTracking\\models";
#elif __linux__
    cubemosModelDir = string(getenv("HOME")) + "/.cubemos/skeleton_tracking/models";
#endif
    return cubemosModelDir;
}

string default_res_dir() {
    string cubemosResDir;
#ifdef _WINDOWS
    cubemosResDir = string(getenv("LOCALAPPDATA")) + "\\Cubemos\\SkeletonTracking\\res";
#elif __linux__
    cubemosResDir = string(getenv("HOME")) + "/.cubemos/skeleton_tracking/res";
#endif
    return cubemosResDir;
}
