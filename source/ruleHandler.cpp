// ruleHandler.cpp
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <fstream>

#include "ruleHandler.h"

RuleHandler::RuleHandler() {
#ifdef _WIN32
    localEyeCANPath += "rules\\";
#elif __APPLE__ || __linux__
    localEyeCANPath += "rules/";
#endif
    std::filesystem::create_directories(localEyeCANPath);
}

int RuleHandler::create(json& info) {
    // Generate a random UUID for the rule and check if already exists
    std::string uuid;
    int status = generate_uuid(uuid);
    if (status != 201) {
        return status;
    }

    json tmp_json; // Temporary json object to store the rule with correct ordered keys
    try {
        tmp_json["id"] = uuid;
        tmp_json["title"] = info["title"];
        tmp_json["description"] = info["description"];
        tmp_json["signals"] = info["signals"];
        tmp_json["settings"] = info["settings"];
    } catch (json::exception& e) {
        return 400;
    }

    // Save the rule to a file
    int isSaved = saveToFile(tmp_json);
    if ( isSaved != 201){
        return isSaved;
    }

    // Return the ID of the rule
    json id_of_rule;
    id_of_rule["id"] = uuid;
    info = id_of_rule;

    return 201;
}

int RuleHandler::edit(json& info, std::string id) {
    const std::string rule_file_path = localEyeCANPath + id + ".json";

    // Check if the file exists
    if (!std::filesystem::exists(rule_file_path)) {
        return 404; // Not Found
    }

    // Overwrite the file with the new JSON information
    try {
        std::ofstream file(rule_file_path);
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

int RuleHandler::remove(std::string id) {
    try {
        const std::string rule_file_path = localEyeCANPath + id + ".json";
        if (std::filesystem::remove(rule_file_path)) {
            return 204; // No Content
        } else {
            return 404; // Not Found
        }
    } catch (const std::filesystem::filesystem_error& e) {
        return 500; // Internal Server Error
    }
}

std::list<json> RuleHandler::get(int page) {
    //TODO get
    return std::list<json>();
}
