cmake_minimum_required(VERSION 3.18)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
project(WORK_MANAGMETN)
set(CMAKE_CXX_STANDARD 17)


#
#set(CMAKE_CXX_FLAGS "-O0" "-Wno-unused-result")
#add_compile_options(O0 -Wall -Wno-unused-result)
#add_compile_options("-O0" "-Wall" "-Wno-unused-result")
add_compile_options("-O0" "-Wno-all")

# test the plateform, must be the unix or the unix-like  

# the option
option(USE_COLOR "use the color print" ON)
if (USE_COLOR)
    add_definitions(-DUSE_COLOR)
    message(STATUS "use the color!")
endif()

# find the python package
find_package(Python3 3.6 REQUIRED Development) # remember to add the Development
if (Python3_FOUND)
    message("find the component")
    message("the python's dirs: ${Python3_INCLUDE_DIRS}")
    message("the python's libs: ${Python3_LIBRARIES}")
    include_directories(${Python3_INCLUDE_DIRS}) # Python.h
endif()

# find the c++ boost package (debain install: sudo apt-get install libboost-all-dev)
find_package(Boost REQUIRED filesystem)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif()

# find the yaml
find_package(yaml-cpp REQUIRED)
INCLUDE_DIRECTORIES(${YAML_CPP_INCLUDE_DIR})
message(STATUS ${YAML_CPP_INCLUDE_DIR})
message(STATUS ${YAML_CPP_LIBRARIES})



# the third party lib in the lib directory: yaml-cpp
#file(GLOB third_party_lib ${CMAKE_SOURCE_DIR}/lib/*)

# include
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/include)

# source files
file(GLOB src_files ${CMAKE_SOURCE_DIR}/src/*.cpp ${CMAKE_SOURCE_DIR}/src/*.h ${CMAKE_SOURCE_DIR}/src/*.hpp)
file(GLOB test_files ${CMAKE_SOURCE_DIR}/src/test.cpp ${CMAKE_SOURCE_DIR}/src/json.hpp)
list(REMOVE_ITEM src_files ${test_files})
# debug
#message(STATUS ${test_files})
#message(STATUS ${src_files})

# target
set (CMAKE_BUILD_TYPE Release)
if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(target_name main_Debug)
elseif (CMAKE_BUILD_TYPE STREQUAL Release) 
    set(target_name main_Release)
else() 
    set(target_name main)
endif()
#
add_executable(${target_name} ${src_files})
add_executable(test ${test_files})
target_link_libraries(${target_name} 
    ${Python3_LIBRARIES} 
    Boost::filesystem 
    readline 
    #${third_party_lib} # the yanml-cpp
    ${YAML_CPP_LIBRARIES}
    )
set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set_target_properties(test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
