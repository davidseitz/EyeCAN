//
// Created by eyecandevelopment on 24.02.25.
//

#ifndef MDFREADER_H
#define MDFREADER_H

struct can_frame;

void receive_frame_data(can_frame* frame);
int readMdf();

#endif // MDFREADER_H
