#include "informationHandler.h"
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#elif __APPLE__
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#elif __linux__
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

InformationHandler::InformationHandler() {
#ifdef _WIN32
    char path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path))) {
        localEyeCANPath = std::string(path) + "\\EyeCan\\";
    }
#elif __APPLE__ || __linux__
    const char* homeDir;
    if ((homeDir = getenv("HOME")) == NULL) {
        homeDir = getpwuid(getuid())->pw_dir;
    }
#ifdef __APPLE__
    localEyeCANPath = std::string(homeDir) + "/Library/Application Support/EyeCan/";
#else
    localEyeCANPath = std::string(homeDir) + "/.local/share/EyeCan/";
#endif
#endif
    // Create the directory if it does not exist
    std::filesystem::create_directories(localEyeCANPath);
}