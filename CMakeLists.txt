cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)

project(webserver)

# collect all executables
set(SOURCES
    src/worker/factory.cpp
    src/client.cpp
    src/main.cpp
    src/server.cpp
)
set(HEADERS
    src/worker/factory.h
    src/client.h
    src/server.h
)

# add executables
add_executable(server ${SOURCES} ${HEADERS})