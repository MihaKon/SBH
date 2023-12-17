cmake_minimum_required(VERSION 3.11.0)
project(SBH VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
    src/main.cpp
    src/SequenceStructure.hpp
    src/SequenceGenerator.hpp
    src/SequenceGenerator.cpp
    src/LevenshteinDistanceCalc.hpp
    src/LevenshteinDistanceCalc.cpp
    )
add_executable(SBH ${SOURCES})
