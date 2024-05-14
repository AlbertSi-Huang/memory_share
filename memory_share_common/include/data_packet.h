#pragma once
#include <string>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <opencv2/opencv.hpp>
const int SHM_SIZE = 20*1024*1024;  // 足够大的共享内存大小

struct DataPacket {
    long long timestamp; // 时间戳
    int imageWidth;      // 图片宽度
    int imageHeight;     // 图片高度
    int imageType;       // 图片数据类型
    int imageSize;       // 图片数据大小
    unsigned char* imageData;     // 图片数据指针
    DataPacket()
    {
        imageData = nullptr;
    }
};
