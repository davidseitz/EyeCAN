#include <articleHandler.h>

ArticleHandler::ArticleHandler() {
#ifdef _WIN32
    localEyeCANPath += "articles\\";
#elif __APPLE__ || __linux__
    localEyeCANPath += "articles/";
#endif
    std::filesystem::create_directories(localEyeCANPath);
}

int ArticleHandler::create(json& info) {
    // Generate a random UUID for the article and check if already exists
    std::string uuid;
    generate_uuid(uuid);

    json tmpJson; // Temporary json object to store the article with correct ordered keys
    try {
        // Check for required keys
        if (!info.contains("title") || !info.contains("date") || !info.contains("author") || !info.contains("content")) {
            return 400;
        }

        // Check if lentgh of date is correct
        // TODO Unix timestamp
        if (((std::string) info["title"]).size() > 128 || ((std::string) info["author"]).size() > 512 || ((std::string) info["content"]).size() > 10000) {
            return 400;
        }

        tmpJson["id"] = uuid;
        tmpJson["title"] = info["title"];
        tmpJson["date"] = info["date"];
        tmpJson["author"] = info["author"];
        tmpJson["content"] = info["content"];
    } catch (json::exception& e) {
        return 400;
    }

    // Save the article to a file
    int isSaved = saveToFile(tmpJson);
    if ( isSaved != 201){
        return isSaved;
    }

    // Return the ID of the article
    json idOfArticle;
    idOfArticle["id"] = uuid;
    info = idOfArticle;

    return 201;
}

int ArticleHandler::get(const int page, json& response) {
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
    response["articles"] = json::array();
    for (const auto& info : information) {
        response["articles"].push_back(info);
    }

    return 200;
}

std::string ArticleHandler::getEyeCANPath() const {
    return localEyeCANPath;
}