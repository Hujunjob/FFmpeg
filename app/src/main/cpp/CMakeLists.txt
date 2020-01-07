cmake_minimum_required(VERSION 3.4.1)

add_library( # Sets the name of the library.
        native-lib
        SHARED
        native-lib.cpp)

include_directories(include)

set(my_lib_path /Users/junhu/git/FFmpeg/app/libs/${ANDROID_ABI})
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -L${my_lib_path}")


find_library( # Sets the name of the path variable.
        log-lib
        log)

target_link_libraries( # Specifies the target library.
        native-lib
        avcodec avfilter avformat avutil swresample swscale
        ${log-lib})