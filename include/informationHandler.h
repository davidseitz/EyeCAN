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
  virtual int edit(json& info, std::string id);
  virtual int remove(std::string id);
  virtual std::list<json> get(int page);
protected:
  std::string localEyeCANPath;
};

#endif //INFORMATIONHANDLER_H