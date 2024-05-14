#include <iostream>
#include <string>
#include <thread>
#include <opencv2/opencv.hpp>
#include "memory_share_reader.h"

bool _needExit = false;

memory_share_reader _reader;

void onThreadHandle()
{
    // 初始化共享内存
    bool bInit = _reader.initMemory("name_fifo_video_stream_0");
    if(!bInit)
    {
        std::cout <<"read mem start failed"<<std::endl;
        return;
    }
    while (!_needExit)
    {
        DataPacket* pack = _reader.readMemory();
        cv::Mat mat(pack->imageHeight,pack->imageWidth,pack->imageType);
        memcpy(mat.data,pack->imageData,pack->imageSize);
        if(mat.empty())
        {
            usleep(2000*10);
            continue;
        }
        cv::putText(mat,std::to_string(pack->timestamp),cv::Point(10,30),1,2,{0,255,0},2);
        cv::resize(mat,mat,cv::Size(mat.cols/2,mat.rows/2));
        cv::imshow("img",mat);
        cv::waitKey(20);
    }
    
}

int main(int argc,char*argv[])
{
    std::thread t(onThreadHandle);
    t.detach();
    char ch = 'a';
    while(true)
    {
        ch = getchar();
        if(ch == 27 || ch == 'q' || ch == 'Q')
        {
            _needExit = true;
            usleep(1000 * 10);
            break;
        }
    }
    return 0;
}