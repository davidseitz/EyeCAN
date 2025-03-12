#ifndef EYECANSERVER_H
#define EYECANSERVER_H

#include <string>
#include <httplib.h>
#include <ruleHandler.h>
#include <articleHandler.h>

class EyeCANServer {
public:
    httplib::Server svr;
    RuleHandler ruleHandler;
    ArticleHandler articleHandler;
    void initServer();

private:
    void initApiEndpoints();
    void initKnowledgebaseEndpoints();
    void initFilterEndpoints();
    void initDatasetEndpoints();
    static std::string readFile(const std::string &filename);
};

#endif //EYECANSERVER_H
