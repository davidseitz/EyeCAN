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
    generate_uuid(uuid);

    json tmp_json; // Temporary json object to store the rule with correct ordered keys
    try {
        // Check for required keys
        if (!info.contains("title") || !info.contains("description") || !info.contains("signals") || !info.contains("settings")) {
            return 400;
        }

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

std::list<json> RuleHandler::get(int page) {
    //TODO get
    return std::list<json>();
}

std::string RuleHandler::getEyeCANPath() const {
    return localEyeCANPath;
}