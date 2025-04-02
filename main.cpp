
#include <eyecanserver.h>
#include "dataFusior.h"

int main() {
  // Test
    DataFusior fusior;
    std::cout << "Reading all Data this may take a while..." << std::endl;
    fusior.readAllData("../dbcExamples/Vehicle.dbc", "../mf4Examples/GFR24-2024-08-14_19-09-12.mdf");
    std::cout << "Data read successfully!" << std::endl;
    return 0;
}


