cmake_minimum_required(VERSION 3.16)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
project(WORK_MANAGMETN)
set(CMAKE_CXX_STANDARD 17)

# the option
option(USE_COLOR "use the color print" ON)
if (USE_COLOR)
    add_definitions(-DUSE_COLOR)
    message(STATUS "use the color!")
endif()

# find the python package
find_package(Python3 3.8 REQUIRED Development) # remember to add the Development
#find_package(Python3 3.8 REQUIRED)
if (Python3_FOUND)
    message("find the component")
    message("the python's dirs: ${Python3_INCLUDE_DIRS}")
    message("the python's libs: ${Python3_LIBRARIES}")
endif()

set(src_code utils.cpp hgj_t.cpp command.cpp regex_utils.cpp main.cpp)
# set(src_code test_regex.cpp)
#set(src_code utils.cpp hgj_t.cpp command.cpp em_python.cpp)
#set(src_code em_python.cpp)
# ----------------------------------------
# include the header
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${Python3_INCLUDE_DIRS}) # Python.h

# about the target
if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(target_name main_Debug)
elseif (CMAKE_BUILD_TYPE STREQUAL Release) 
    set(target_name main_Release)
else() 
    set(target_name main)
endif()
add_executable(${target_name} ${src_code})
target_link_libraries(${target_name} ${Python3_LIBRARIES} readline)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
