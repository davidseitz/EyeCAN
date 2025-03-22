#include <datasetHandler.h>

DatasetHandler::DatasetHandler() {
#ifdef _WIN32
    localEyeCANPath += "data\\";
#elif __APPLE__ || __linux__
    localEyeCANPath += "data/";
#endif
    std::filesystem::create_directories(localEyeCANPath);
}

int DatasetHandler::create(json& info) {
    // Generate a random UUID for the article and check if already exists
    std::string uuid;
    generateUuid(uuid);

    json tmpJson; // Temporary json object to store the article with correct ordered keys
    try {
        // Check for required keys
        if (!info.contains("title") || !info.contains("description") || !info.contains("selectedSignalValues") || !info.contains("signals")) {
            return 400;
        }

        // Check if lentgh of date is correct
        if (static_cast<std::string>(info["title"]).size() > 128 || static_cast<std::string>(info["description"]).size() > 300) {
            return 400;
        }

        tmpJson["id"] = uuid;
        tmpJson["title"] = info["title"];
        tmpJson["description"] = info["description"];
        tmpJson["selectedSignalValues"] = info["selectedSignalValues"];
        tmpJson["signals"] = info["signals"];
    } catch (const json::exception&) {
        return 400;
    }

    // Save the article to a file
    int isSaved = saveToFile(tmpJson);
    if ( isSaved != 201){
        return isSaved;
    }

    // Return the article
    info = tmpJson;

    return 201;
}

int DatasetHandler::get(const int page, json& response) {
    if (page < 1) {
        return 400;
    }
    response["page"] = page;

    std::list<json> information;
    try {
        int result = getFiles(page, information, response);
        if (result != 0) {
            return result;
        }
    } catch (std::filesystem::filesystem_error&) {
        return 500;
    } catch (json::exception&) {
        return 400;
    }

    // Check if files are found on this page
    if (information.empty()) {
        return 204;
    }

    // Generate array of requested filters
    response["datasets"] = json::array();
    for (const auto& info : information) {
        response["datasets"].push_back(info);
    }

    return 200;
}

std::string DatasetHandler::getEyeCANPath() const {
    return localEyeCANPath;
}