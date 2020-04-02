cmake_minimum_required(VERSION 3.0)

# build libfreenect2 as subdirectory (name: freenect2)
add_subdirectory(external/libfreenect2)
set(freenect2_INCLUDE_DIRS
    external/libfreenect2/include
    ${CMAKE_CURRENT_BINARY_DIR}/external/libfreenect2)

# make static library from cvdrone sources
file(GLOB CVDRONES_SOURCES external/cvdrones/src/ardrone/*.cpp)
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

# main include dependencies
set(MAIN_INCLUDE_DIRS
    ${freenect2_INCLUDE_DIRS}
    ${EIGEN3_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS})

set(MAIN_LINK_LIBRARIES
    freenect2
    ${OpenCV_LIBRARIES}
    basic
    pthread
    )

# ar include dependencies
set(AR_INCLUDE_DIRS
    ${OpenCV_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    external/cvdrones/src)

set(AR_LINK_LIBRARIES
    ${BOOST_LIBRARIES}
    pthread
    cvdrones basic)

# visualizer dependencies
set(VISUALIZER_INCLUDE_DIRS
    ${OPENGL_INCLUDE_DIR}
    ${GLUT_INCLUDE_DIR}
    ${EIGEN3_INCLUDE_DIRS}
    ${graphic_INCLUDE_DIRS})

set(VISUALIZER_LINK_LIBRARIES
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARY}
    ${BOOST_LIBRARIES}
    pthread
    graphic
    basic)
