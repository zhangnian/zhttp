# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project(zhttp)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -lrt")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -lrt")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# 查找当前目录下的所有源文件,并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)

# 指定依赖的第三方头文件路径
include_directories(/data/DevComm/libevent-2.0.22/include)
include_directories(/data/DevComm/zookeeper-3.4.6/include)
include_directories(/data/DevComm/rapidjson)

# 指定依赖的第三方库文件路径
LINK_DIRECTORIES(/usr/local/lib)
LINK_DIRECTORIES(/usr/lib64)
LINK_DIRECTORIES(/data/DevComm/libevent-2.0.22/lib)
LINK_DIRECTORIES(/data/DevComm/zookeeper-3.4.6/lib)


# 指定生成目标
add_executable(zhttp ${DIR_SRCS})

# 指定依赖的第三方静态/动态库
target_link_libraries(zhttp Threads::Threads -ldl libevent.a libzookeeper_mt.a)
