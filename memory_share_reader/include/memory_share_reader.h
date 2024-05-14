#pragma once 
#include "data_packet.h"

class memory_share_reader{
public:
    memory_share_reader();
    ~memory_share_reader();

    bool initMemory(std::string name);
    DataPacket *readMemory();
    private:
    unsigned char* _shm_ptr;
    DataPacket _pack;
    int _lastImageSize;
    int _shm_id ;
};