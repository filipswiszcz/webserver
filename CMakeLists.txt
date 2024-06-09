cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)

project(webserver)

# collect all executables
set(SOURCES
    src/util/logger.cpp
    src/worker/factory.cpp
    src/client.cpp
    src/main.cpp
    src/server.cpp
)
set(HEADERS
    src/util/logger.hpp
    src/worker/factory.h
    src/client.h
    src/server.h
)

# add executables
add_executable(server ${SOURCES} ${HEADERS})