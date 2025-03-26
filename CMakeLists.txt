cmake_minimum_required(VERSION 3.31.1)
set(PROJECT_NAME compresser)
set(SOURCE_FILES src/main.c src/compression.c src/file_reading.c)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
