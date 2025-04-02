//
// Created by eyecandevelopment on 25.02.25.
//

#ifndef DBCREADER_H
#define DBCREADER_H

#include "dbcppp/CApi.h"
#include <dbcppp/Network.h>
#include <iostream>
#include <memory>
#include <fstream>


class dbcReader {
public:
    dbcReader() = default;
    void printCANSignals(const std::string& dbcFilePath);
};



#endif //DBCREADER_H
