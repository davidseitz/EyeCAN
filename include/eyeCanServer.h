#ifndef EYECANSERVER_H
#define EYECANSERVER_H

#include <string>
#include <httplib.h>
#include <ruleHandler.h>
#include <articleHandler.h>
#include <datasetHandler.h>

class EyeCANServer {
public:
    httplib::Server svr;
    RuleHandler ruleHandler;
    ArticleHandler articleHandler;
    DatasetHandler datasetHandler;
    void initServer();

private:
    void initApiEndpoints();
    void initKnowledgebaseEndpoints();
    void initFilterEndpoints();
    void initDatasetEndpoints();
    static std::string readFile(const std::string &filename);
};

#endif //EYECANSERVER_H
