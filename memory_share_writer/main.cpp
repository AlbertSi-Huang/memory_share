#include <iostream>
#include <string>
#include <thread>
#include <opencv2/opencv.hpp>
#include "memory_share_writer.h"

bool _needExit = false;
cv::VideoCapture g_cap;

memory_share_writer _writer;

void onThreadHandle()
{
    //打开视频
    bool open = g_cap.open("123.mp4");
    std::cout <<"open finish : " <<open <<std::endl;
    if (!open)
    {
        std::cout <<"open video error" <<std::endl;
        return;
    }
    int m_fps = g_cap.get(cv::CAP_PROP_FPS);
    if (m_fps <= 0)
    {
        m_fps = 25;
    }
    int _fcount = g_cap.get(cv::CAP_PROP_FRAME_COUNT);
    
    // 初始化共享内存
    bool bOpen = _writer.initMemory("name_fifo_video_stream_0");
    if(!bOpen)
    {
        std::cout <<"writer mem start failed" <<std::endl;
    }
    while(true)
    {
        if(_needExit)break;
        int cur = g_cap.get(cv::CAP_PROP_POS_FRAMES); 
        if (cur > (_fcount - 1) ) {
            std::cout <<"设置当前帧为0，循环播放。"<<std::endl;
            g_cap.set(cv::CAP_PROP_POS_FRAMES, 0); // 设置当前帧为0，循环播放。
            continue;
        }
        
        cv::Mat mat;
        if ((!g_cap.read(mat)) || mat.empty()) {
            std::cout <<"mat is empty"<<std::endl;
            usleep(5);
            continue;
        }
        _writer.writeToMemory(mat);
        // cv::imshow("img",mat);
        cv::waitKey(1000 / m_fps);

        

    }
    std::cout <<"跳出while" <<std::endl;
}

int main(int argc, char *argv[])
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