cmake_minimum_required(VERSION 2.8)
project( colorDetection )

include(FetchContent)
FetchContent_Declare(port_audio GIT_REPOSITORY "https://github.com/PortAudio/portaudio.git")
FetchContent_MakeAvailable(port_audio)

add_executable(colorDetection pitcher.h colorTracking.cpp port_audio.h oscillator.cpp oscillator.h saw.cpp saw.h circBuffer.cpp circBuffer.h)
target_include_directories(colorDetection PRIVATE "D:\\opencv\\build\\include")
target_link_libraries(colorDetection PRIVATE "D:\\opencv\\build\\x64\\vc15\\lib\\opencv_world451d.lib")
target_link_libraries(colorDetection PRIVATE portaudio_static)
target_compile_features(colorDetection PRIVATE cxx_std_17)