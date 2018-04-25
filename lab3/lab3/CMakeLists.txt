cmake_minimum_required(VERSION 2.8)


project(lab3)

find_package (Threads)

file(GLOB SRC . *.c)
add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
