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
find_package(Eigen3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    )

set(MAIN_INCLUDE_DIRS
    ${freenect2_INCLUDE_DIRS}
    ${EIGEN3_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    kinect esp32 control drone basic params common control/impl
    )

set(MAIN_LINK_LIBRARIES
    ${freenect2_LIBRARIES}
    ${OpenCV_LIBRARIES}
    ${Boost_LIBRARY_DIRS}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    )

set(AR_INCLUDE_DIRS
    ${OpenCV_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    basic common
    ${EXTERNAL_DIR}/cvdrones/src)

set(AR_LINK_LIBRARIES
    ${Boost_LIBRARY_DIRS}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    pthread)

set(VISUALIZER_INCLUDE_DIRS
    ${OPENGL_INCLUDE_DIR}
    ${GLUT_INCLUDE_DIR}
    ${EIGEN3_INCLUDE_DIRS}
    visualizer common basic)

set(VISUALIZER_LINK_LIBRARIES
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARY}
    ${Boost_LIBRARY_DIRS}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    pthread)

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

# set main executable
file(GLOB MAIN_SOURCES main.cpp)
file(GLOB MAIN_MODULE_SOURCES kinect/*.cpp esp32/*.cpp control/*.cpp drone/*.cpp basic/*.cpp control/impl/*.cpp)

add_executable(main ${MAIN_SOURCES} ${MAIN_MODULE_SOURCES})

target_include_directories(main PRIVATE ${MAIN_INCLUDE_DIRS})
target_link_libraries(main ${MAIN_LINK_LIBRARIES})

# set ar executable
file(GLOB AR_SOURCES main_ar.cpp)
file(GLOB AR_MODULE_SOURCES basic/*.cpp)

add_executable(ar ${AR_SOURCES} ${AR_MODULE_SOURCES})

target_include_directories(ar PRIVATE ${AR_INCLUDE_DIRS})
target_link_libraries(ar ${AR_LINK_LIBRARIES} cvdrones)

# set visualizer executable
file(GLOB VISUALIZER_SOURCES main_viz.cpp)
file(GLOB VISUALIZER_MODULE_SOURCES visualizer/*.cpp basic.cpp)

add_executable(visualizer ${VISUALIZER_SOURCES} ${VISUALIZER_MODULE_SOURCES})

target_include_directories(visualizer PRIVATE ${VISUALIZER_INCLUDE_DIRS})
target_link_libraries(visualizer ${VISUALIZER_LINK_LIBRARIES})


# set main test executables
file(GLOB MAIN_TEST_SOURCES testsuite/main/*.cpp)

foreach(TEST_SOURCE ${MAIN_TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    message(${TEST_NAME})

    add_executable(${TEST_NAME} ${TEST_SOURCE} ${MAIN_MODULE_SOURCES})
    target_include_directories(${TEST_NAME} PRIVATE ${MAIN_INCLUDE_DIRS} )
    target_link_libraries(${TEST_NAME} ${MAIN_LINK_LIBRARIES})
endforeach()

# set ar test executables
file(GLOB AR_TEST_SOURCES testsuite/ar/*.cpp)

foreach(TEST_SOURCE ${AR_TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    message(${TEST_NAME})

    add_executable(${TEST_NAME} ${TEST_SOURCE} ${AR_MODULE_SOURCES})
    target_include_directories(${TEST_NAME} PRIVATE ${AR_INCLUDE_DIRS} )
    target_link_libraries(${TEST_NAME}
        ${AR_LINK_LIBRARIES}
        cvdrones)
endforeach()

# set visualizer test executables
file(GLOB VISUALIZER_TEST_SOURCES testsuite/visualizer/*.cpp)

foreach(TEST_SOURCE ${VISUALIZER_TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    message(${TEST_NAME})

    add_executable(${TEST_NAME} ${TEST_SOURCE} ${VISUALIZER_MODULE_SOURCES})
    target_include_directories(${TEST_NAME} PRIVATE ${VISUALIZER_INCLUDE_DIRS} )
    target_link_libraries(${TEST_NAME}
        ${VISUALIZER_LINK_LIBRARIES}
        )
endforeach()
