cmake_minimum_required(VERSION 3.30)

project(win_framework)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(include)

set(SOURCE_FILES
    source/Button.c
    source/ComboBox.c
    source/DynamicArray.c
    source/Edit.c
    source/main.c
    source/ProgressBar.c
    source/Window.c
    source/resources.rc)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_compile_options(${PROJECT_NAME} PRIVATE -O2)
target_link_options(${PROJECT_NAME} PRIVATE -s -static-libgcc -static -mwindows)
target_link_libraries(${PROJECT_NAME} PRIVATE -lgdi32 -lcomctl32)
