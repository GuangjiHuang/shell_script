set(source_file src/opencv_4.cpp)
cmake_minimum_required(VERSION 3.16)
set(CXX_STANDARD 11)
project(TEST_OPENCV_FUNCTION)

# find the package: opencv
set(OpenCV_DIR /usr/local/opencv-4.5-release)
#set(OpenCV_DIR /usr/local/opencv-4.5-release/lib/cmake)
find_package(OpenCV REQUIRED)

# set the source file
get_filename_component(target_name ${source_file} NAME_WE)

# the target 
add_executable(${target_name} ${source_file})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
target_include_directories(${target_name} PUBLIC  ${OpenCV_INCLUDE_DIRS})
target_link_libraries(${target_name} ${OpenCV_LIBS})
