cmake_minimum_required(VERSION 3.30)

project(win_framework)

add_executable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_sources(${PROJECT_NAME} PRIVATE
                ./source/Button.c
                ./source/Child.c
                ./source/ComboBox.c
                ./source/display.c
                ./source/DynamicArray.c
                ./source/Edit.c
                ./source/filename.c
                ./source/ListBox.c
                ./source/main.c
                ./source/Menu.c
                ./source/ProgressBar.c
                ./source/Tab.c
                ./source/Window.c
                ./source/windowProcFunc.c
                ./source/resources.rc)

target_compile_options(${PROJECT_NAME} PRIVATE -O2)
# Debugging build that can print to console. To do: This right
# target_link_options(${PROJECT_NAME} PRIVATE -s -static-libgcc -static)
# Release that doesn't need the console.
target_link_options(${PROJECT_NAME} PRIVATE -s -mwindows -static-libgcc -static)
target_link_libraries(${PROJECT_NAME} PRIVATE -lcomdlg32 -lgdi32 -lcomctl32)
