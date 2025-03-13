#ifndef INFORMATIONHANDLER_H
#define INFORMATIONHANDLER_H

#include <string>
#include <nlohmann/json.hpp>
#include <list>

using json = nlohmann::ordered_json;

class InformationHandler {
public:
    InformationHandler();
    virtual ~InformationHandler() = default;
    virtual int create(json& info) = 0;
    virtual int edit(const json& info, const std::string& id);
    virtual int remove(const std::string& id);
    virtual int get(const int page, json& response) = 0;
protected:
    std::string localEyeCANPath;
    int saveToFile(json& info) const;
    int generateUuid(std::string& uuid) const;
    int getFiles(const int page, std::list<json>& information, json& response) const;
};

#endif //INFORMATIONHANDLER_H
