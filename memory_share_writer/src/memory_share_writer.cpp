#include "memory_share_writer.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <chrono>

memory_share_writer::memory_share_writer()
    : _shm_ptr(nullptr)
{
}

memory_share_writer::~memory_share_writer()
{
    // 解除映射

    if (munmap(_shm_ptr, SHM_SIZE) == -1)
    {

        std::cerr << "Failed to unmap shared memory" << std::endl;
    }

    // 关闭共享内存对象

    if (close(_shm_id) == -1)
    {

        std::cerr << "Failed to close shared memory file descriptor" << std::endl;
    }
}

bool memory_share_writer::initMemory(const std::string &memName)
{
    key_t shm_key = ftok("/tmp", 'R');
    _shm_id = shm_open(memName.c_str(),  O_RDWR, 0666);
    if (_shm_id == -1)
    {
        std::cout <<"O_WRONLY 失败" <<std::endl;
        // 如果创建失败，则改为打开
        _shm_id = shm_open(memName.c_str(), O_CREAT | O_RDWR, 0666);
        if (_shm_id == -1)
        {
            std::cout << "打开写共享内存失败" << std::endl;
            return false;
        }
        
    }
    if (ftruncate(_shm_id, SHM_SIZE) == -1)
        {
            std::cout << "开辟空间失败" << std::endl;
            return false;
        }
    _shm_ptr = (unsigned char *)mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, _shm_id, 0);
    if (_shm_ptr == MAP_FAILED)
    {
        std::cout << "内存映射失败" << std::endl;
        return false;
    }

    return true;
}

void memory_share_writer::writeToMemory(const cv::Mat &mat)
{
    try
    {
        long long tick = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        // 设置时间戳
        memcpy(static_cast<unsigned char *>(_shm_ptr), &tick, sizeof(long long));
        memcpy(static_cast<unsigned char *>(_shm_ptr) + sizeof(long long), &mat.cols, sizeof(int));
        memcpy(static_cast<unsigned char *>(_shm_ptr) + sizeof(long long) + sizeof(int), &mat.rows, sizeof(int));
        int nType = mat.type();
        memcpy(static_cast<unsigned char *>(_shm_ptr) + sizeof(long long) + 2 * sizeof(int), &nType, sizeof(int));
        int imageSize = mat.cols * mat.rows * mat.channels();
        memcpy(static_cast<unsigned char *>(_shm_ptr) + sizeof(long long) + 3 * sizeof(int), &imageSize, sizeof(int));
        memcpy(static_cast<unsigned char *>(_shm_ptr) + sizeof(long long) + 4 * sizeof(int), mat.data, imageSize);
    }
    catch (const std::exception &)
    {
    }
}
