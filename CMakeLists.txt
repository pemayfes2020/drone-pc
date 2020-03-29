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
file(GLOB CVDRONES_SOURCES ${EXTERNAL_DIR}/cvdrones/src/ardrone/*.cpp)
add_library(cvdrones STATIC ${CVDRONES_SOURCES})
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

# main include dependencies
set(MAIN_INCLUDE_DIRS
    ${freenect2_INCLUDE_DIRS}
    ${EIGEN3_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS})

set(MAIN_LINK_LIBRARIES
    ${freenect2_LIBRARIES}
    ${OpenCV_LIBRARIES}
    basic
    )

# ar include dependencies
set(AR_INCLUDE_DIRS
    ${OpenCV_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${EXTERNAL_DIR}/cvdrones/src)

set(AR_LINK_LIBRARIES
    ${BOOST_LIBRARIES}
    pthread
    cvdrones basic)

# visualizer dependencies
set(VISUALIZER_INCLUDE_DIRS
    ${OPENGL_INCLUDE_DIR}
    ${GLUT_INCLUDE_DIR}
    ${EIGEN3_INCLUDE_DIRS})

set(VISUALIZER_LINK_LIBRARIES
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARY}
    ${BOOST_LIBRARIES}
    pthread
    basic)

# set executables for real environment
foreach(MAKE_TARGET MAIN AR VISUALIZER)

    string(TOLOWER ${MAKE_TARGET} MAKE_TARGET_LOWER)

    ## set main executable
    file(GLOB ${MAKE_TARGET}_MODULE_SOURCES modules_${MAKE_TARGET_LOWER}/*/*.cpp)

    add_executable(${MAKE_TARGET_LOWER} ${MAKE_TARGET_LOWER}.cpp ${${MAKE_TARGET}_MODULE_SOURCES})

    target_include_directories(${MAKE_TARGET_LOWER} PRIVATE ${${MAKE_TARGET}_INCLUDE_DIRS} modules_${MAKE_TARGET_LOWER})
    target_link_libraries(${MAKE_TARGET_LOWER} ${${MAKE_TARGET}_LINK_LIBRARIES})

    ## set test executables
    file(GLOB ${MAKE_TARGET}_TEST_SOURCES testsuite/${MAKE_TARGET_LOWER}/*.cpp)
    foreach(TEST_SOURCE ${${MAKE_TARGET}_TEST_SOURCES})
        get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

        add_executable(${TEST_NAME} ${TEST_SOURCE} ${${MAKE_TARGET}_MODULE_SOURCES})
        target_include_directories(${TEST_NAME} PRIVATE ${${MAKE_TARGET}_INCLUDE_DIRS} modules_${MAKE_TARGET_LOWER})
        target_link_libraries(${TEST_NAME} ${${MAKE_TARGET}_LINK_LIBRARIES})
    endforeach()

endforeach()

# set simulator executables
if (DEFINED BUILD_SIM)
    add_definitions(-D${BUILD_SIM})
else()
    set(${BUILD_SIM} off)
endif()

if(BUILD_SIM)
    message("Targets for simulator environment will be built")
else()
    message("Targets for simulator environment won't be built")
    RETURN()
endif()

## add library for simulator core
file(GLOB SIM_CORE_SOURCES simulator/core/src/*.cpp)
add_library(simulator_core STATIC ${SIM_CORE_SOURCES})
target_include_directories(simulator_core PRIVATE simulator/core/include)
target_link_libraries(simulator_core
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARY}
    ${OpenCV_LIBRARIES})

## add library for libfreenect2_sim
file(GLOB LIBFREENECT2_SIM_SOURCES simulator/libfreenect2_sim/src/*.cpp)
add_library(libfreenect2_sim STATIC ${LIBFREENECT2_SIM_SOURCES})
target_include_directories(libfreenect2_sim PRIVATE 
    ${OpenCV_INCLUDE_DIRS}
    simulator/libfreenect2_sim/include
    simulator/core/include)

target_link_libraries(libfreenect2_sim
    ${OpenCV_LIBRARIES}
    simulator_core)

## add library for cvdrones_sim
file(GLOB CVDRONES_SIM_SOURCES simulator/cvdrones_sim/src/*.cpp)
add_library(cvdrones_sim STATIC ${CVDRONES_SIM_SOURCES})
set_target_properties(cvdrones_sim PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(cvdrones_sim PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    simulator/cvdrones_sim/include
    simulator/core/include)
target_link_libraries(cvdrones_sim
    ${OpenCV_LIBRARIES}
    simulator_core)

## override main dependencies
set(MAIN_INCLUDE_DIRS
    simulator/libfreenect2_sim/include
    ${EIGEN3_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS})

set(MAIN_LINK_LIBRARIES
    libfreenect2_sim 
    ${OpenCV_LIBRARIES}
    basic
    )

## override ar include directories
set(AR_INCLUDE_DIRS
    ${OpenCV_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    simulator/cvdrones_sim/include)

set(AR_LINK_LIBRARIES
    ${BOOST_LIBRARIES}
    pthread
    cvdrones_sim basic)

## executables
foreach(MAKE_TARGET MAIN AR VISUALIZER)

    string(TOLOWER ${MAKE_TARGET} MAKE_TARGET_LOWER)

    ### set main executable
    add_executable(sim_${MAKE_TARGET_LOWER} ${MAKE_TARGET}.cpp ${${MAKE_TARGET}_MODULE_SOURCES})

    target_include_directories(sim_${MAKE_TARGET_LOWER} PRIVATE ${${MAKE_TARGET}_INCLUDE_DIRS} modules_${MAKE_TARGET_LOWER})
    target_link_libraries(sim_${MAKE_TARGET_LOWER} ${${MAKE_TARGET}_LINK_LIBRARIES})

    ### set test executables
    file(GLOB SIM_${MAKE_TARGET}_TEST_SOURCES testsuite/${MAKE_TARGET_LOWER}/*.cpp)
    foreach(TEST_SOURCE ${SIM_${MAKE_TARGET}_TEST_SOURCES})
        get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

        add_executable(sim_${TEST_NAME} ${TEST_SOURCE} ${${MAKE_TARGET}_MODULE_SOURCES})
        target_include_directories(sim_${TEST_NAME} PRIVATE ${${MAKE_TARGET}_INCLUDE_DIRS} modules_${MAKE_TARGET_LOWER})
        target_link_libraries(sim_${TEST_NAME} ${${MAKE_TARGET}_LINK_LIBRARIES})
    endforeach()

endforeach()
