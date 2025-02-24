#ifndef EYECANSERVER_H
#define EYECANSERVER_H

#include <string>
#include <httplib.h>

class EyeCANServer {
public:
    httplib::Server svr;
    void initServer();

private:
    void initApiEndpoints();
    void initKnowledgebaseEndpoints();
    void initFilterEndpoints();
    void initDatasetEndpoints();
    static std::string readFile(const std::string &filename);
};

#endif //EYECANSERVER_H

