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

set(PROJECT_NAME paperui)

# 查找所有源文件和头文件
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/*.cpp src/*.c)
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS include/*.h include/*.hpp)
if(MSVC)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS PAPERUI_EXPORTS)
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
            message("输出：${basefilter_name}/${filter},${file}\n")
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


    file(GLOB_RECURSE PRIVATE_SOURCE_FILES CONFIGURE_DEPENDS private/src/*.cpp private/src/*.c)
    file(GLOB_RECURSE PRIVATE_HEADER_FILES CONFIGURE_DEPENDS private/include/*.h private/include/*.hpp)

    foreach(NEW_FILE ${PRIVATE_SOURCE_FILES})
        list(APPEND SOURCE_FILES ${NEW_FILE})
        source_group("Source Files//Private//src" FILES ${NEW_FILE})
    endforeach()
    foreach(NEW_FILE ${PRIVATE_HEADER_FILES})
        list(APPEND HEADER_FILES ${NEW_FILE})
        source_group("Header Files//Private//include" FILES ${NEW_FILE})
    endforeach()

    # message("开始输出最终结果：")
    # foreach(NEW_FILE ${SOURCE_FILES})
    #     message("输出：${NEW_FILE}")
    # endforeach()
    # foreach(NEW_FILE ${HEADER_FILES})
    #     message("输出：${NEW_FILE}")
    # endforeach()
    # message(${HEADER_FILES})
    # message(${SOURCE_FILES})
    # message(${PRIVATE_HEADER_FILES})
    # message(${PRIVATE_SOURCE_FILES})
    # filter_files("${PRIVATE_HEADER_FILES}" "Header Files\\Private" "${CMAKE_CURRENT_SOURCE_DIR}")
    # filter_files("${PRIVATE_SOURCE_FILES}" "Source Files\\Private" "${CMAKE_CURRENT_SOURCE_DIR}")

    
endif(MSVC)

# 静态库
include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

link_directories(
    
)

link_libraries(

)

# add_executable(openssldemo WIN32 ${HEADER_FILES} ${SOURCE_FILES})
# add_executable(openssldemo ${HEADER_FILES} ${SOURCE_FILES})
# add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
add_library(paperui SHARED ${HEADER_FILES} ${SOURCE_FILES})
# 动态库
target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_directories(${PROJECT_NAME} PRIVATE
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    d2d1
    dwrite
)


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/build/paperui/Debug/paperui.dll" "${CMAKE_SOURCE_DIR}/build/example1/Debug/"
    COMMAND_EXPAND_LISTS
)




