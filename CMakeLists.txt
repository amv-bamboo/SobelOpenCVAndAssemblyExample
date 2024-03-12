cmake_minimum_required(VERSION 3.15)

project(lab_asm_mmx_lena)

find_package( OpenCV CONFIG REQUIRED )

set(SOURCE_EXE main.cpp)

add_executable(main ${SOURCE_EXE})

target_link_libraries(main PRIVATE ${OpenCV_LIBS})
