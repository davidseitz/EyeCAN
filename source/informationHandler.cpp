#include "informationHandler.h"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

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
    if (SUCCEEDED(SHGetFolderPathA(nullptr, CSIDL_LOCAL_APPDATA, nullptr, 0, path))) {
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

int InformationHandler::edit(const json& info, const std::string& id) {
    const std::string informationFilePath = localEyeCANPath + id + ".json";

    // Check if the file exists
    if (!std::filesystem::exists(informationFilePath)) {
        return 404; // Not Found
    }

    // Overwrite the file with the new JSON information
    try {
        std::ofstream file(informationFilePath);
        if (!file.is_open()) {
            throw std::ios_base::failure("Failed to open file");
        }
        file << info.dump(4);
        file.close();
    } catch (std::ios_base::failure& e) {
        return 500; // Internal Server Error
    } catch (json::exception& e) {
        return 400; // Bad Request
    }

    return 200;
}

int InformationHandler::remove(const std::string& id) {
    try {
        const std::string informationFilePath = localEyeCANPath + id + ".json";
        if (std::filesystem::remove(informationFilePath)) {
            return 204; // No Content
        } else {
            return 404; // Not Found
        }
    } catch (const std::filesystem::filesystem_error& e) {
        return 500; // Internal Server Error
    }
}

int InformationHandler::saveToFile(json& info) const
{
    try {
        const std::string filename = info["id"];
        const std::string informationFilePath = localEyeCANPath + filename + ".json";
        std::ofstream file(informationFilePath);
        if (!file.is_open()) {
            throw std::ios_base::failure("Failed to open file");
        }
        file << info.dump(4);
        file.close();
    } catch (std::ios_base::failure& e) { //TODO http status codes
        return 500;
    } catch (json::exception& e) {
        return 400;
    }
    return 201;
}

int InformationHandler::generate_uuid(std::string& uuid) const
{
    boost::uuids::random_generator generator;
    const boost::uuids::uuid gen_uuid = generator();
    uuid = to_string(gen_uuid);

    // Check if the file already exists
    if (std::filesystem::exists(localEyeCANPath + uuid + ".json"))
    {
        return 404;
    }
    return 201;
}