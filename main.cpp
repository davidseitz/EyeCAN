
#include <eyecanserver.h>
#include <mdfAndDbcBasics.h>

int main() {
    readMdf();
    EyeCANServer server;
    server.initServer();
    return 0;
}