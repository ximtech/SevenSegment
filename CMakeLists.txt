cmake_minimum_required(VERSION 3.20)
project(SEVEN_SEGMENT VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)

set(SEVEN_SEGMENT_DIRECTORY
        ${SEVEN_SEGMENT_SOURCE_DIR}
        ${SEVEN_SEGMENT_SOURCE_DIR}/include
        CACHE STRING "Seven segment LED directories include to the main project" FORCE)

set(SEVEN_SEGMENT_SOURCES
        ${SEVEN_SEGMENT_SOURCE_DIR}/RotaryEncoder.c
        ${SEVEN_SEGMENT_SOURCE_DIR}/include/RotaryEncoder.h
        CACHE STRING "Seven segment LED source files include to the main project" FORCE)