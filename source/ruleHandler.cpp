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

RuleHandler::~RuleHandler() {
    // Destructor implementation
}

int RuleHandler::create(json& info) {
    boost::uuids::random_generator generator;
    boost::uuids::uuid uuid = generator();
    std::string gen_uuid = boost::uuids::to_string(uuid);

    json tmp_json;
    try {
        tmp_json["id"] = gen_uuid;
        tmp_json["title"] = info["title"];
        tmp_json["description"] = info["description"];
        tmp_json["signals"] = info["signals"];
        tmp_json["settings"] = info["settings"];
    } catch (json::exception& e) {
        return 404;
    }

    int isSaved = saveToFile(tmp_json);
    if ( isSaved != 201){
        return isSaved;
    }


    json id_of_rule;
    id_of_rule["id"] = gen_uuid;
    info = id_of_rule;

    return 201;
}

int RuleHandler::edit(json& info, std::string id) {
    //TODO edit
    return 0;
}

int RuleHandler::remove(std::string id) {
    //TODO remove
    return 0;
}

std::list<json> RuleHandler::get(int page) {
    //TODO get
    return std::list<json>();
}

int RuleHandler::saveToFile(json& info) {
    try {
        std::string filename = info["id"];
        std::string rule_file_path = localEyeCANPath + filename + ".json";
        std::ofstream file(rule_file_path);
        if (!file.is_open()) {
            throw std::ios_base::failure("Failed to open file");
        }
        file << info.dump(4);
        file.close();
    } catch (std::ios_base::failure& e) {
        return 500;
    } catch (json::exception& e) {
        return 404;
    }
    return 201;
}