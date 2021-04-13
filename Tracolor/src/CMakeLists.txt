cmake_minimum_required(VERSION 2.8)
project( Tracolor )

include(FetchContent)
FetchContent_Declare(port_audio GIT_REPOSITORY "https://github.com/PortAudio/portaudio.git")
FetchContent_MakeAvailable(port_audio)

add_executable(Tracolor pitch_shifter/pitcher.h main.cpp pitch_shifter/port_audio.h pitch_shifter/oscillator.cpp pitch_shifter/oscillator.h
               pitch_shifter/saw.cpp pitch_shifter/saw.h pitch_shifter/circBuffer.cpp pitch_shifter/circBuffer.h colorDetection.h
               reverse_delay/delay.cpp reverse_delay/delay.h reverse_delay/delayLine.cpp
               reverse_delay/delayLine.h reverse_delay/reverse.cpp reverse_delay/reverse.h reverse_delay/jackModuleAdapter.h)

target_include_directories(Tracolor PRIVATE "C:\\Users\\steve\\OneDrive\\Documenten\\Programming\\lib\\opencv\\build\\include")
target_link_libraries(Tracolor PRIVATE "C:\\Users\\steve\\OneDrive\\Documenten\\Programming\\lib\\opencv\\build\\x64\\vc15\\lib\\opencv_world451d.lib")
target_link_libraries(Tracolor PRIVATE portaudio_static)
target_compile_features(Tracolor PRIVATE cxx_std_17)
