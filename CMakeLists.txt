cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME paperui)

project(${PROJECT_NAME})

set(ACTIVEPROJECT example1)

add_subdirectory(example1)
add_subdirectory(paperui)


if(MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${ACTIVEPROJECT})
endif(MSVC)



