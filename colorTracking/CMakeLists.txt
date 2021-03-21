cmake_minimum_required(VERSION 2.8)
project( DisplayImage )

add_executable( DisplayImage test.cpp )
target_include_directories(DisplayImage PRIVATE "D:\\opencv\\build\\include")
target_link_libraries(DisplayImage PRIVATE "D:\\opencv\\build\\x64\\vc15\\lib\\opencv_world451d.lib")

#[[cmake_minimum_required(VERSION 3.17)
project(colorDetection)

include(FetchContent)
FetchContent_Declare(opencv_lib GIT_REPOSITORY "https://github.com/opencv/opencv.git")
FetchContent_MakeAvailable(opencv_lib)

add_executable(colorDetection test.cpp)
target_include_directories(colorDetection PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(colorDetection PRIVATE ${OpenCV_LIBS})]]#