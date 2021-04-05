cmake_minimum_required(VERSION 2.8)
project( colorDetection )

include(FetchContent)
FetchContent_Declare(port_audio GIT_REPOSITORY "https://github.com/PortAudio/portaudio.git")
FetchContent_MakeAvailable(port_audio)

add_executable(colorDetection colorTracking.cpp port_audio.h oscillator.cpp oscillator.h saw.cpp saw.h circBuffer.cpp circBuffer.h)
target_include_directories(colorDetection PRIVATE "D:\\opencv\\build\\include")
target_link_libraries(colorDetection PRIVATE "D:\\opencv\\build\\x64\\vc15\\lib\\opencv_world451d.lib")
target_link_libraries(colorDetection PRIVATE portaudio_static)
target_compile_features(colorDetection PRIVATE cxx_std_17)


#[[cmake_minimum_required(VERSION 3.17)
project(colorDetection)

include(FetchContent)
FetchContent_Declare(opencv_lib GIT_REPOSITORY "https://github.com/opencv/opencv.git")
FetchContent_MakeAvailable(opencv_lib)

add_executable(colorDetection test.cpp)
target_include_directories(colorDetection PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(colorDetection PRIVATE ${OpenCV_LIBS})]]#