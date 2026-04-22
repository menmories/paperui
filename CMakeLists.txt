cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME paperui)

project(${PROJECT_NAME})


# 设置输出目录，确保库和可执行文件在同一目录，便于运行
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(ACTIVEPROJECT example1)

add_subdirectory(example1)
add_subdirectory(paperui)


if(MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${ACTIVEPROJECT})
endif(MSVC)



