cmake_minimum_required(VERSION 2.8)

project(lab_1)
file(GLOB SRC . *.cpp)
add_executable(${PROJECT_NAME} ${SRC})
