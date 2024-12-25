# add_compile_definitions(TINYUI_API)

# file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp src/*.c include/*.h include/*.c)

#set_property(GLOBAL PROPERTY USE_FOLDERS ON)
#message(${PROJECT_SOURCE_DIR})

# CMake的string(REPLACE)命令用于替换一个字符串中的指定子字符串。

# 其基本语法为：
# ```
# string(REPLACE )
# ```

# 其中：
# - ``：要被替换的子字符串。
# - ``：替换 `` 的字符串。
# - ``：要进行替换操作的原始字符串。
# - ``：替换完成后的输出字符串。

# 例如，要将字符串"Hello World"中的"World"替换为"CMake"，可以这样使用string(REPLACE)命令：
# ```cmake
# string(REPLACE "World" "CMake" new_string "Hello World")
# message(${new_string})
# ```

# AI bot
# https://chatgptonline.tech/zh/

set(PROJECT_NAME example1)


# 在这个例子中，最终输出的`new_string`将是"Hello CMake"。
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/*.cpp src/*.c)
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS include/*.h include/*.hpp)
if(MSVC)
    add_definitions("-DUNICODE" "-D_UNICODE")
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS DLL_IMPORT)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    #创建一个文件过滤器
    macro(filter_files files basefilter_name remove_dir)
        foreach(file ${files})
            # message(${file})
            # message("remove_dir=" ${remove_dir})
            string(REPLACE ${remove_dir} "" filter_name ${file})
            string(REGEX REPLACE "/[^/]*$" "" filter ${filter_name})
            # message("filter_name=" ${filter_name}) 
            # message("filter=" ${filter})
            source_group("${basefilter_name}/${filter}" FILES ${file})
            # message("filename=" ${trimmed_variable} "\n")
        endforeach()
    endmacro()

    set(FILTER_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include")
    set(FILTER_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src")

    # 将头文件添加到Visual Studio的"Header Files"分类下
    filter_files("${HEADER_FILES}" "Header Files" ${FILTER_HEADER})
    # 将源文件添加到Visual Studio的"Source Files"分类下
    filter_files("${SOURCE_FILES}" "Source Files" ${FILTER_SOURCE})
endif(MSVC)

# 静态库
# include_directories(
#     include
#     ../thirdparty/glfw/include
# )

# link_directories(
#     ../thirdparty/glfw/lib/Debug/
# )

link_libraries(
    # ws2_32.lib
)

# add_executable(openssldemo WIN32 ${HEADER_FILES} ${SOURCE_FILES})
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
# add_library(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})


# 动态库
target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/paperui/include"
)


set(BUILD_DIR "${CMAKE_SOURCE_DIR}/build")
message(build_dir="${BUILD_DIR}")
target_link_directories(${PROJECT_NAME} PRIVATE
    "${BUILD_DIR}/${PROJECT_NAME}/Debug/"
)

message("${BUILD_DIR}/${PROJECT_NAME}/Debug/")

target_link_libraries(${PROJECT_NAME} PRIVATE
    paperui
)


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/build/${PROJECT_NAME}/Debug/paperui.dll" "${CMAKE_SOURCE_DIR}/build/example1/Debug/"
    COMMAND_EXPAND_LISTS
)

# Visual Studio中的调试工作目录（即exe程序内的“./”所指的目录，仅在VS调试中有效）
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build/example1/Debug")
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME})
