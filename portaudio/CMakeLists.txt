cmake_minimum_required(VERSION 3.17)

project(port_audio_test)

include(FetchContent)

FetchContent_Declare(port_audio GIT_REPOSITORY "https://github.com/PortAudio/portaudio.git")
FetchContent_MakeAvailable(port_audio)

add_executable(test main.cpp port_audio.h oscillator.cpp oscillator.h saw.cpp saw.h)

target_compile_features(test PRIVATE cxx_std_17)

target_link_libraries(test PRIVATE portaudio_static)