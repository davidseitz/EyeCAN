// ruleHandler.h
#ifndef RULEHANDLER_H
#define RULEHANDLER_H

#include "informationHandler.h"

class RuleHandler : public InformationHandler {
public:
    RuleHandler();
    ~RuleHandler() override;
    int create(json& info) override;
    int edit(json& info, std::string id) override;
    int remove(std::string id) override;
    std::list<json> get(int page) override;

private:
    int saveToFile(json& info);
};

#endif // RULEHANDLER_H