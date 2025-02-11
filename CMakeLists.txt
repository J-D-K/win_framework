cmake_minimum_required(VERSION 3.30)

project(win_framework)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(include)

set(SOURCE_FILES
    source/DynamicArray.c
    source/main.c
    source/Window.c
    source/resources.rc)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_compile_options(${PROJECT_NAME} PRIVATE -O2 -s)
target_link_options(${PROJECT_NAME} PRIVATE -mwindows)
target_link_libraries(${PROJECT_NAME} PRIVATE -lgdi32 -lcomctl32)
