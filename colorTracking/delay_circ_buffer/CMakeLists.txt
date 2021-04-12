cmake_minimum_required(VERSION 3.17)
project( tracolor )

include(FetchContent)
FetchContent_Declare(port_audio GIT_REPOSITORY "https://github.com/PortAudio/portaudio.git")
FetchContent_MakeAvailable(port_audio)

add_executable(tracolor main.cpp delay.cpp delay.h delayLine.cpp delayLine.h port_audio.h reverse.cpp reverse.h jackModuleAdapter.h)
target_include_directories(tracolor PRIVATE "D:\\opencv\\build\\include")
target_link_libraries(tracolor PRIVATE "D:\\opencv\\build\\x64\\vc15\\lib\\opencv_world451d.lib")
target_link_libraries(tracolor PRIVATE portaudio_static)
target_compile_features(tracolor PRIVATE cxx_std_17)