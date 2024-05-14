#ifndef MEMORY_SHARE_WRITER_H
#define MEMORY_SHARE_WRITER_H
#include "data_packet.h"
class memory_share_writer
{
public:
    memory_share_writer();
    ~memory_share_writer();

    bool initMemory(const std::string& memName);
    void writeToMemory(const cv::Mat& mat);

private:
    unsigned char* _shm_ptr;
    int _shm_id;
};

#endif // MEMORY_SHARE_WRITER_H
