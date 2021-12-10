cmake_minimum_required(VERSION 3.20)

# FTXUI
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
        GIT_TAG v0.11
        )

FetchContent_GetProperties(ftxui)
if (NOT ftxui_POPULATED)
    FetchContent_Populate(ftxui)
    add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif ()

# Project

project(IRCpp
        VERSION 0.1.1
        LANGUAGES CXX
        DESCRIPTION "Simple IRC realization on C++")

include(generate_product_version.cmake)
generate_product_version(
        VERSION_FILE
        NAME "IRCpp Project"
        ICON "${CMAKE_SOURCE_DIR}/icons/common.ico"
        VERSION_MAJOR ${CMAKE_PROJECT_VERSION_MAJOR}
        VERSION_MINOR ${CMAKE_PROJECT_VERSION_MINOR}
        VERSION_PATCH ${CMAKE_PROJECT_VERSION_PATCH}
        VERSION_REVISION ${BUILD_REVISION}
        COMPANY_NAME "Ivan Petrov (LazyTechwork)"
        COMPANY_COPYRIGHT "(c) 2021, Ivan Petrov and IISD Foundation"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CXX_STANDARD_LIBRARIES}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")

set(COMMON_SOURCE_FILES src/common/SocketConnection.cpp src/common/Logger.cpp src/common/Utils.cpp)
set(COMMON_HEADER_FILES src/common/SocketConnection.h src/common/SocketConstants.h src/common/Logger.h src/common/Utils.h src/common/CommandDefinitions.h)

set(CLIENT_SOURCE_FILES IRCppClient.cpp src/client/ClientSocket.cpp src/client/CmdProcessor.cpp)
set(CLIENT_HEADER_FILES src/client/ClientSocket.h src/client/CmdProcessor.h src/client/GUIReferences.h)

set(SERVER_SOURCE_FILES IRCppServer.cpp src/server/ServerSocket.cpp src/server/CmdProcessor.cpp)
set(SERVER_HEADER_FILES src/server/ServerSocket.h src/server/CmdProcessor.h)

add_executable(IRCppClient ${COMMON_HEADER_FILES} ${COMMON_SOURCE_FILES} ${CLIENT_HEADER_FILES} ${CLIENT_SOURCE_FILES} ${VERSION_FILE})
add_executable(IRCppServer ${COMMON_HEADER_FILES} ${COMMON_SOURCE_FILES} ${SERVER_HEADER_FILES} ${SERVER_SOURCE_FILES} ${VERSION_FILE})

target_include_directories(IRCppClient PRIVATE src/client src/common)
target_include_directories(IRCppServer PRIVATE src/server src/common)
target_link_libraries(IRCppClient
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component
        )