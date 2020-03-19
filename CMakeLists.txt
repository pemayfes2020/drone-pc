cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_COMPILER "/usr/bin/g++" CACHE STRING "g++ compiler" FORCE)
set(CMAKE_C_COMPILER "/usr/bin/gcc" CACHE STRING "gcc compiler" FORCE)
set(CMAKE_C_FLAG "${CMAKE_C_FLAGS} -g -O0")
set(CMAKE_CXX_FLAG "${CMAKE_C_FLAGS} -g -O0 -lboost_system")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(DRONE_PC LANGUAGES C CXX)

set(EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)

# find packages
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME     ON)
find_package(Boost 1.65.1 REQUIRED
             COMPONENTS date_time system)

find_package(freenect2 REQUIRED)
find_package(OpenCV REQUIRED)

set(EXEC_INCLUE_DIRS
    ${freenect2_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    kinect esp32 control drone common
    ${EXTERNAL_DIR}/cvdrone/src/
    )

set(EXEC_LINK_LIBRARIES
    ${freenect2_LIBRARIES}
    ${OpenCV_LIBRARIES}
    ${Boost_LIBRARY_DIRS}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY})

# make static library from cvdrone sources
file(GLOB CVDRONE_SOURCES ${EXTERNAL_DIR}/cvdrone/src/ardrone/*.cpp)
add_library(cvdrone STATIC ${CVDRONE_SOURCES})
set_target_properties(cvdrone PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(cvdrone
        m
        pthread
        avutil
        avformat
        avcodec
        swscale
        ${OpenCV_LIBRARIES}
    )

# set main executable
file(GLOB MAIN_SOURCES main.cpp)
file(GLOB MODULE_SOURCES kinect/*.cpp esp32/*.cpp control/*.cpp drone/*.cpp common/*.cpp)

add_executable(main ${MAIN_SOURCES} ${MODULE_SOURCES})

target_include_directories(main PRIVATE ${EXEC_INCLUE_DIRS})
target_link_libraries(main 
    ${EXEC_LINK_LIBRARIES}
    cvdrone
    )

# set test executables
file(GLOB TEST_SOURCES testsuite/*.cpp)

foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    message(${TEST_NAME})

    add_executable(${TEST_NAME} ${TEST_SOURCE} ${MODULE_SOURCES})
    target_include_directories(${TEST_NAME} PRIVATE ${EXEC_INCLUE_DIRS} )
    target_link_libraries(${TEST_NAME}
        ${EXEC_LINK_LIBRARIES} cvdrone)
endforeach()
