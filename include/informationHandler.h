//
// Created by robin on 05.03.2025.
//

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
  virtual std::list<json> get(int page) = 0;
  virtual int edit(const json& info, const std::string& id);
  virtual int remove(const std::string& id);
protected:
  std::string localEyeCANPath;
  int saveToFile(json& info) const;
  int generate_uuid(std::string& uuid) const;
  int getFiles(const int page, std::list<json>& information, json& response) const;
};

#endif //INFORMATIONHANDLER_H