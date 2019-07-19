cmake_minimum_required(VERSION 3.14)
project(tergen)

set(CMAKE_CXX_STANDARD 17)

add_executable(tergen Src/main.cpp
        Src/Split/Processor.cpp
        Src/Split/Processor.h
        Src/Readers/SrtmReader.cpp
        Src/Readers/SrtmReader.h
        Src/Model/Map.cpp
        Src/Model/Map.h
#        Src/Model/Mesh.cpp
#        Src/Model/Mesh.h
#        Src/Model/Point.cpp
#        Src/Model/Point.h
#        Src/Model/Triangle.cpp
#        Src/Model/Triangle.h
        Src/Utils/Utils.cpp Src/Utils/Utils.h)