#include "memory_share_reader.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <chrono>

memory_share_reader::memory_share_reader()
:_shm_ptr(nullptr)
,_lastImageSize(0)
{
}

memory_share_reader::~memory_share_reader()
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

bool memory_share_reader::initMemory(std::string name)
{
    _shm_id = shm_open(name.c_str(),O_RDWR ,0666);
    if(_shm_id == -1){
        std::cout <<"打开失败，需要创建"<<std::endl;
        _shm_id = shm_open(name.c_str(), O_CREAT | O_RDWR,0666);
        if(_shm_id == -1)
        {
            std::cout <<"打开读共享内存失败"<<std::endl;
            return false;
        }
        //开辟空间
        if(ftruncate(_shm_id,SHM_SIZE) == -1)
        {   
            std::cout <<"开辟空间 失败"<<std::endl;
            return false;
        }
    }

    //映射
    _shm_ptr = (unsigned char*)mmap(0, SHM_SIZE, PROT_READ , MAP_SHARED, _shm_id, 0);
        if (_shm_ptr == MAP_FAILED) {
            std::cout <<"映射空间 失败"<<std::endl;
            return false;
        }

    return true;
}

DataPacket *memory_share_reader::readMemory()
{
    long long tick =0;// std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    // 设置时间戳
    
    memcpy(&tick,static_cast<unsigned char*>(_shm_ptr)  , sizeof(long long));
    int nW = 0;
    
    memcpy(&nW,static_cast<unsigned char*>(_shm_ptr) + sizeof(long long),sizeof(int));
    int nH = 0;
    
    memcpy(&nH,static_cast<unsigned char*>(_shm_ptr) + sizeof(long long) + sizeof(int),sizeof(int));
    int nType = 0;
    
    memcpy(&nType,static_cast<unsigned char*>(_shm_ptr) + sizeof(long long) + 2*sizeof(int),sizeof(int));

    int imageSize =0;
    
    memcpy(&imageSize,static_cast<unsigned char*>(_shm_ptr) + sizeof(long long) + 3*sizeof(int),sizeof(int));
    
    if(_lastImageSize != imageSize)
    {
        if(_pack.imageData)
        {
            delete _pack.imageData;
            _pack.imageData = nullptr;
        }
        
        _pack.imageData = new unsigned char[imageSize];
    }

    _lastImageSize = imageSize;
    _pack.imageHeight = nH;
    _pack.imageWidth = nW;
    _pack.imageSize = imageSize;
    _pack.imageType = nType;
    
    memcpy(_pack.imageData ,static_cast<unsigned char*>(_shm_ptr) + sizeof(long long) + 4*sizeof(int),imageSize);

    _pack.timestamp = tick;
    return &_pack;
}