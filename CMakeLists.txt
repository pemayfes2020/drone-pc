cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_COMPILER "/usr/bin/g++" CACHE STRING "g++ compiler" FORCE)
set(CMAKE_C_COMPILER "/usr/bin/gcc" CACHE STRING "gcc compiler" FORCE)
set(CMAKE_C_FLAG "${CMAKE_C_FLAGS} -g -O0")
set(CMAKE_CXX_FLAG "${CMAKE_C_FLAGS} -g -O0 -lboost_system")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(DRONE_PC LANGUAGES C CXX)

# find packages
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME     ON)
find_package(Boost 1.65.1 REQUIRED
             COMPONENTS date_time system)

find_package(freenect2 REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(
    ${freenect2_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    kinect esp32 control
    )

set(COMMON_LINK_LIBRARIES
        ${freenect2_LIBRARIES}
        ${OpenCV_LIBRARIES}
        ${Boost_LIBRARY_DIRS}
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY})

# set main executable
file(GLOB MAIN_SOURCES main.cpp)
file(GLOB MODULE_SOURCES kinect/*.cpp esp32/*.cpp control/*.cpp)

add_executable(main ${MAIN_SOURCES} ${MODULE_SOURCES})

target_link_libraries(main PRIVATE ${COMMON_LINK_LIBRARIES})

# set test executables
file(GLOB TEST_SOURCES testsuite/*.cpp)

foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    message(${TEST_NAME})

    add_executable(${TEST_NAME} ${TEST_SOURCE} ${MODULE_SOURCES})
    target_link_libraries(${TEST_NAME} PRIVATE ${COMMON_LINK_LIBRARIES} )
endforeach()
