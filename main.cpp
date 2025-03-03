
#include <eyecanserver.h>
#include "dataFusior.h"

int main() {
    DataFusior fusior;
    fusior.changeDbcData();
    EyeCANServer server;
    server.initServer();
    return 0;
}
