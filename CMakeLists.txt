cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_COMPILER "/usr/bin/g++" CACHE STRING "g++ compiler" FORCE)
set(CMAKE_C_COMPILER "/usr/bin/gcc" CACHE STRING "gcc compiler" FORCE)
set(CMAKE_C_FLAG "${CMAKE_C_FLAGS} -g -O0")
set(CMAKE_CXX_FLAG "${CMAKE_C_FLAGS} -g -O0")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(DRONE_PC LANGUAGES C CXX)

set(EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)

# find packages
find_package(Boost 1.65.1 REQUIRED
             COMPONENTS date_time system)

find_package(freenect2 REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)


set(BOOST_LIBRARIES
    boost_system boost_date_time)

# make static library from cvdrone sources
file(GLOB CVDRONE_SOURCES ${EXTERNAL_DIR}/cvdrones/src/ardrone/*.cpp)
add_library(cvdrones STATIC ${CVDRONE_SOURCES})
set_target_properties(cvdrones PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(cvdrones
        m
        pthread
        avutil
        avformat
        avcodec
        swscale
        ${OpenCV_LIBRARIES}
    )

# make static library for basic programs
file(GLOB BASIC_SOURCES basic/*.cpp)
add_library(basic STATIC ${BASIC_SOURCES})
set_target_properties(basic PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(basic ${BOOST_LIBRARIES})

# common include directories
include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${EXTERNAL_DIR}/cmdline
    common basic
    )

# main include directories
set(MAIN_INCLUDE_DIRS
    ${freenect2_INCLUDE_DIRS}
    ${EIGEN3_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    modules_main
    )

set(MAIN_LINK_LIBRARIES
    ${freenect2_LIBRARIES}
    ${OpenCV_LIBRARIES}
    basic
    )

file(GLOB MAIN_MODULE_SOURCES modules_main/*/*.cpp)

# ar include directories
set(AR_INCLUDE_DIRS
    ${OpenCV_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    modules_ar
    ${EXTERNAL_DIR}/cvdrones/src)

set(AR_LINK_LIBRARIES
    ${BOOST_LIBRARIES}
    pthread
    cvdrones basic)

file(GLOB AR_MODULE_SOURCES modules_ar/*/*.cpp)

# visualizer include directories
set(VISUALIZER_INCLUDE_DIRS
    ${OPENGL_INCLUDE_DIR}
    ${GLUT_INCLUDE_DIR}
    ${EIGEN3_INCLUDE_DIRS}
    modules_visualizer)

set(VISUALIZER_LINK_LIBRARIES
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARY}
    ${BOOST_LIBRARIES}
    pthread
    basic)

set(MAKE_TARGETS
    MAIN
    AR
    VISUALIZER
    )

# set executables
foreach(MAKE_TARGET MAIN AR VISUALIZER)

    string(TOLOWER ${MAKE_TARGET} MAKE_TARGET_LOWER)

    # set main executable
    file(GLOB ${MAKE_TARGET}_SOURCES ${MAKE_TARGET_LOWER}.cpp)
    file(GLOB VISUALIZER_MODULE_SOURCES modules_${MAKE_TARGET_LOWER}/*/*.cpp)

    add_executable(${MAKE_TARGET_LOWER} ${${MAKE_TARGET}_SOURCES} ${${MAKE_TARGET}_MODULE_SOURCES})

    target_include_directories(${MAKE_TARGET_LOWER} PRIVATE ${${MAKE_TARGET}_INCLUDE_DIRS})
    target_link_libraries(${MAKE_TARGET_LOWER} ${${MAKE_TARGET}_LINK_LIBRARIES})

    # set test executables
    file(GLOB ${MAKE_TARGET}_TEST_SOURCES testsuite/${MAKE_TARGET_LOWER}/*.cpp)
    foreach(TEST_SOURCE ${${MAKE_TARGET}_TEST_SOURCES})
        get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

        add_executable(${TEST_NAME} ${TEST_SOURCE} ${${MAKE_TARGET}_MODULE_SOURCES})
        target_include_directories(${TEST_NAME} PRIVATE ${${MAKE_TARGET}_INCLUDE_DIRS} )
        target_link_libraries(${TEST_NAME} ${${MAKE_TARGET}_LINK_LIBRARIES})
    endforeach()

endforeach()
