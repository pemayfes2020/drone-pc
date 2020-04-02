cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_COMPILER "/usr/bin/g++" CACHE STRING "g++ compiler" FORCE)
set(CMAKE_C_COMPILER "/usr/bin/gcc" CACHE STRING "gcc compiler" FORCE)
set(CMAKE_C_FLAG "${CMAKE_C_FLAGS} -g -O0 -fmax-errors=5 -Wfatal-erros")
set(CMAKE_CXX_FLAG "${CMAKE_C_FLAGS} -g -O0 -fmax-errors=5 -Wfatal-erros -std=c++17")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(DRONE_PC LANGUAGES C CXX)

# find packages
find_package(Boost 1.65.1 REQUIRED
             COMPONENTS date_time system)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

set(BOOST_LIBRARIES
    boost_system boost_date_time)

# make static library for basic programs
file(GLOB BASIC_SOURCES basic/*.cpp)
add_library(basic STATIC ${BASIC_SOURCES})
set_target_properties(basic PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(basic ${BOOST_LIBRARIES})

# build graphic as subdirectory (name: graphic)
add_subdirectory(external/drone-gl)
set(graphic_INCLUDE_DIRS
    external/drone-gl/include)

# common include directories
include_directories(
    ${OpenCV_INCLUDE_DIRS}
    external/cmdline
    common basic)

# set simulator executables
if (DEFINED SIM)
    add_definitions(-D${SIM})
else()
    set(${SIM} off)
endif()

if(SIM)
    message("Targets for simulator environment will be built")
    include(cmake_library/sim.cmake)
    set(TARGET_PREFIX sim_)
else()
    message("Targets for real environment will be built")
    include(cmake_library/real.cmake)
    set(TARGET_PREFIX "")
endif()

## executables
foreach(MAKE_TARGET MAIN AR VISUALIZER)

    string(TOLOWER ${MAKE_TARGET} MAKE_TARGET_LOWER)

    file(GLOB ${MAKE_TARGET}_MODULE_SOURCES modules_${MAKE_TARGET_LOWER}/*/*.cpp)

    ### set main executable
    add_executable(${TARGET_PREFIX}${MAKE_TARGET_LOWER} ${MAKE_TARGET}.cpp ${${MAKE_TARGET}_MODULE_SOURCES})

    target_include_directories(${TARGET_PREFIX}${MAKE_TARGET_LOWER} PRIVATE ${${MAKE_TARGET}_INCLUDE_DIRS} modules_${MAKE_TARGET_LOWER})
    target_link_libraries(${TARGET_PREFIX}${MAKE_TARGET_LOWER} ${${MAKE_TARGET}_LINK_LIBRARIES})

    ### set test executables
    file(GLOB ${MAKE_TARGET}_TEST_SOURCES testsuite/${MAKE_TARGET_LOWER}/*.cpp)
    foreach(TEST_SOURCE ${${MAKE_TARGET}_TEST_SOURCES})
        get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

        add_executable(${TARGET_PREFIX}${TEST_NAME} ${TEST_SOURCE} ${${MAKE_TARGET}_MODULE_SOURCES})
        target_include_directories(${TARGET_PREFIX}${TEST_NAME} PRIVATE ${${MAKE_TARGET}_INCLUDE_DIRS} modules_${MAKE_TARGET_LOWER})
        target_link_libraries(${TARGET_PREFIX}${TEST_NAME} ${${MAKE_TARGET}_LINK_LIBRARIES})
    endforeach()

endforeach()
