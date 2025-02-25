//
// Created by eyecandevelopment on 25.02.25.
//

#ifndef DATAFUSIOR_H
#define DATAFUSIOR_H

#include <nlohmann/json.hpp>

class DataFusior {
public:

    void changeDbcData();
    void changeMdfData();

    nlohmann::ordered_json getFusedData();


private:
    void fuseData();

    nlohmann::ordered_json m_fusedData;
};
#endif //DATAFUSIOR_H
