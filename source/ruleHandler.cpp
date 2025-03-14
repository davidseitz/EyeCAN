#include <ruleHandler.h>

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
    generateUuid(uuid);

    json tmpJson; // Temporary json object to store the rule with correct ordered keys
    try {
        // Check for required keys
        if (!info.contains("title") || !info.contains("description") || !info.contains("signals") || !info.contains("settings")) {
            return 400;
        }

        tmpJson["id"] = uuid;
        tmpJson["title"] = info["title"];
        tmpJson["description"] = info["description"];
        tmpJson["signals"] = info["signals"];
        tmpJson["settings"] = info["settings"];
    } catch (json::exception& e) {
        return 400;
    }

    // Save the rule to a file
    int isSaved = saveToFile(tmpJson);
    if ( isSaved != 201){
        return isSaved;
    }

    // Return the ID of the rule
    json idOfRule;
    idOfRule["id"] = uuid;
    info = idOfRule;

    return 201;
}

int RuleHandler::get(const int page, json& response) {
    if (page < 1) {
        return 400;
    }
    response["page"] = page;

    std::list<json> information;
    try {
        // Get requested files
        getFiles(page, information, response);
    } catch (std::filesystem::filesystem_error& e) {
        return 500;
    } catch (json::exception& e) {
        return 400;
    }

    // Check if files are found on this page
    if (information.empty()) {
        return 204;
    }

    // Generate array of requested filters
    response["filters"] = json::array();
    for (const auto& info : information) {
        response["filters"].push_back(info);
    }
    return 200;
}

std::string RuleHandler::getEyeCANPath() const {
    return localEyeCANPath;
}