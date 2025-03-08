
#include <eyecanserver.h>
#include "dataFusior.h"

int main() {
    DataFusior fusior;
    for (int i = 0; i < 1000; i++ ){
        fusior.readChannelDataByCanId(i);
        }
    return 0;
}
