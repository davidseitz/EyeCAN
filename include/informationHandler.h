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
  virtual int edit(json& info, std::string id) = 0;
  virtual int remove(std::string id) = 0;
  virtual std::list<json> get(int page) = 0;
protected:
  std::string localEyeCANPath;
  int saveToFile(json& info) const;
  int generate_uuid(std::string& uuid) const;
};

#endif //INFORMATIONHANDLER_H