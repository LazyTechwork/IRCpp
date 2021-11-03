cmake_minimum_required(VERSION 3.20)
project(IRCpp
        VERSION 1.0.0
        LANGUAGES CXX
        DESCRIPTION "Simple IRC realization on C++")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CXX_STANDARD_LIBRARIES}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")

set(COMMON_SOURCE_FILES src/common/SocketConnection.cpp)
set(COMMON_HEADER_FILES src/common/SocketConnection.h)

set(CLIENT_SOURCE_FILES IRCppClient.cpp)
set(CLIENT_HEADER_FILES)

set(SERVER_SOURCE_FILES IRCppServer.cpp src/server/ServerSocket.cpp)
set(SERVER_HEADER_FILES src/server/ServerSocket.h)

add_executable(IRCppClient ${CLIENT_SOURCE_FILES} ${CLIENT_HEADER_FILES} ${COMMON_SOURCE_FILES} ${COMMON_HEADER_FILES})
add_executable(IRCppServer ${SERVER_SOURCE_FILES} ${SERVER_HEADER_FILES} ${COMMON_SOURCE_FILES} ${COMMON_HEADER_FILES})

target_include_directories(IRCppClient PRIVATE src/client src/common)
target_include_directories(IRCppServer PRIVATE src/server src/common)

