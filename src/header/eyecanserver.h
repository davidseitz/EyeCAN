#ifndef EYECANSERVER_H
#define EYECANSERVER_H

#include <httplib.h>
#include <string>

class EyeCANServer {
public:
    httplib::Server svr;
    void initServer();

private:
    void initApiEndpoints();
    static std::string readFile(const std::string &filename);
};

#endif //EYECANSERVER_H
