
#include <eyecanserver.h>
#include <mdfAndDbcBasics.h>

int main() {
    printCANSignals("../dbcExamples/Vehicle.dbc");
    EyeCANServer server;
    server.initServer();
    return 0;
}