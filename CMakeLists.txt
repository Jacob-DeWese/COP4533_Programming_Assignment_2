cmake_minimum_required(VERSION 3.30)
project(COP4533_Prog_Assi2)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")

add_executable(COP4533_Prog_Assi2 src/main.cpp
        src/Cache.h
        src/CacheAlgorithms.cpp)
