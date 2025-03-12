// ruleHandler.h
#ifndef RULEHANDLER_H
#define RULEHANDLER_H

#include <informationHandler.h>

class RuleHandler : public InformationHandler {
public:
    RuleHandler();
    ~RuleHandler() override = default;
    int create(json& info) override;
    int get(const int page, json& response) override;
    std::string getEyeCANPath() const;
};

#endif // RULEHANDLER_H