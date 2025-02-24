//
// Created by eyecandevelopment on 24.02.25.
//

#ifndef MDFANDDBCBASICS_H
#define MDFANDDBCBASICS_H

#include <string>

struct can_frame;

void receive_frame_data(can_frame* frame);
void printCANSignals(const std::string& dbcFilePath);
int readMdf();

#endif // MDFANDDBCBASICS_H