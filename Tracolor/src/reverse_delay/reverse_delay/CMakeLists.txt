cmake_minimum_required(VERSION 3.17)
project(revDelayRemake)

include(FetchContent)
FetchContent_Declare(port_audio GIT_REPOSITORY "https://github.com/PortAudio/portaudio.git")
FetchContent_MakeAvailable(port_audio)

add_executable(revDelayRemake delayLine.cpp main.cpp delayLine.cpp delayLine.h portAudio.h jackModuleAdapter.h delay.cpp delay.h)

target_link_libraries(revDelayRemake PRIVATE portaudio_static_)
target_compile_features(revDelayRemake PRIVATE cxx_std_17)