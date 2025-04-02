//
// Created by eyecandevelopment on 25.02.25.
//

#ifndef DATAFUSIOR_H
#define DATAFUSIOR_H

#include <fstream>
#include <nlohmann/json.hpp>

class DataFusior {
public:

    DataFusior(); // Constructor
    void changeDbcData(); // lTODO
    void changeMdfData(); // lTODO
    ~DataFusior();
    nlohmann::ordered_json getFusedData(); // Getter
    void readAllData( std::string dbcFilepath, std::string mdfFilepath); // Parse data of both DBC and MDF files

private:
    void fuseData(); // Combine DBC and MDF data
    std::ofstream outFile;
    nlohmann::ordered_json jsonData;
    nlohmann::ordered_json m_fusedData;
};
#endif //DATAFUSIOR_H
