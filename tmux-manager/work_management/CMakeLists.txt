cmake_minimum_required(VERSION 3.16)
project(TEST_OPENCV)

set(CMAKE_CXX_STANDARD 17)

# find the python package
find_package(Python3 3.8 REQUIRED Development) # remember to add the Development
#find_package(Python3 3.8 REQUIRED)
if (Python3_FOUND)
    message("find the component")
    message("the python's dirs: ${Python3_INCLUDE_DIRS}")
    message("the python's libs: ${Python3_LIBRARIES}")
endif()

set(src_code utils.cpp hgj_t.cpp command.cpp main.cpp)
#set(src_code utils.cpp hgj_t.cpp command.cpp em_python.cpp)
#set(src_code em_python.cpp)
# ----------------------------------------
# include the header
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${Python3_INCLUDE_DIRS}) # Python.h

# about the target
add_executable(main ${src_code})
target_link_libraries(main ${Python3_LIBRARIES})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
