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
    ~InformationHandler();
    virtual json create();
    virtual json edit();
    virtual int remove();
    virtual std::list<json> get();
  protected:
    std::string localKnowledgebase;
};

#endif //INFORMATIONHANDLER_H
