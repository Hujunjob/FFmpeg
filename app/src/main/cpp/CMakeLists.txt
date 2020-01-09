cmake_minimum_required(VERSION 3.4.1)

file(GLOB SOURCE *.cpp)
add_library( # Sets the name of the library.
        hujunplayer
        SHARED
        ${SOURCE})

include_directories(include)

#需要手动设置当前项目的目录
set(project_path /Users/junhu/git/FFmpeg)

set(my_lib_path ${project_path}/app/libs/${ANDROID_ABI})
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -L${my_lib_path}")


find_library( # Sets the name of the path variable.
        log-lib
        log)

target_link_libraries( # Specifies the target library.
        hujunplayer
        z
        #依赖库的放置顺序也会影响运行
        avfilter avformat avcodec avutil swresample swscale
        ${log-lib}
        OpenSLES
        android   #窗口ANativeWindow库
        )