cmake_minimum_required(VERSION 3.20)
project(IRCpp)

set(CMAKE_CXX_STANDARD 23)

add_executable(IRCppClient IRCppClient.cpp)
add_executable(IRCppServer IRCppServer.cpp)
