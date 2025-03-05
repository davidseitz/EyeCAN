// ruleHandler.h
#ifndef RULEHANDLER_H
#define RULEHANDLER_H

#include "informationHandler.h"

class RuleHandler : public InformationHandler {
public:
    RuleHandler();
    ~RuleHandler();
    json create(json info) override;
    json edit(json info, std::string id) override;
    int remove(std::string id) override;
    std::list<json> get(int page) override;
};

#endif // RULEHANDLER_H