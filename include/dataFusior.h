//
// Created by eyecandevelopment on 25.02.25.
//

#ifndef DATAFUSIOR_H
#define DATAFUSIOR_H

#include <fstream>
#include <nlohmann/json.hpp>

class DataFusior {
public:
    DataFusior();
    void changeDbcData();
    void changeMdfData();
    ~DataFusior();
    nlohmann::ordered_json getFusedData();
    void readChannelDataByCanId(uint32_t canId);

private:
    void fuseData();
    std::ofstream outFile;
    nlohmann::ordered_json jsonData;
    nlohmann::ordered_json m_fusedData;
};
#endif //DATAFUSIOR_H
