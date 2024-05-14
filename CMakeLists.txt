cmake_minimum_required(VERSION 3.10)  

# 设置项目名称和版本  

project(memory_share_writer VERSION 1.0)  
# 查找OpenCV库  

# 添加编译选项  
add_compile_options(-std=c++17)  
find_package(OpenCV REQUIRED)  
# 包含OpenCV头文件目录  
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(memory_share_common/include/
    memory_share_writer/include/
    memory_share_reader/include/)

file(GLOB writer_head_list 
    memory_share_common/include/*.h
    memory_share_writer/include/*.h)
file(GLOB writer_source_list
    memory_share_writer/src/*.cpp)

    file(GLOB reader_head_list 
    memory_share_common/include/*.h
    memory_share_reader/include/*.h)
file(GLOB reader_source_list
    memory_share_reader/src/*.cpp)

# 添加源代码文件  

add_executable(memory_share_writer memory_share_writer/main.cpp
    ${writer_head_list}
    ${writer_source_list})  

add_executable(memory_share_reader memory_share_reader/main.cpp
    ${reader_head_list}
    ${reader_source_list})  

# file(writer_head_list )
# 链接OpenCV库  

target_link_libraries(memory_share_writer PUBLIC ${OpenCV_LIBS})  
target_link_libraries(memory_share_reader PUBLIC ${OpenCV_LIBS})  

