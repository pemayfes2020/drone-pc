cmake_minimum_required(VERSION 3.0)

# add library for simulator core
file(GLOB SIM_CORE_SOURCES simulator/core/src/*.cpp)
add_library(simulator_core STATIC ${SIM_CORE_SOURCES})
target_include_directories(simulator_core PRIVATE simulator/core/include basic)
target_link_libraries(simulator_core
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARY}
    ${OpenCV_LIBRARIES}
    basic)

# add library for libfreenect2_sim
file(GLOB LIBFREENECT2_SIM_SOURCES simulator/libfreenect2_sim/src/*.cpp)
add_library(libfreenect2_sim STATIC ${LIBFREENECT2_SIM_SOURCES})
target_include_directories(libfreenect2_sim PRIVATE 
    ${OpenCV_INCLUDE_DIRS}
    simulator/libfreenect2_sim/include
    simulator/core/include)

target_link_libraries(libfreenect2_sim
    ${OpenCV_LIBRARIES}
    simulator_core)

# add library for cvdrones_sim
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


# simulator main dependencies
set(MAIN_INCLUDE_DIRS
    simulator/libfreenect2_sim/include
    ${EIGEN3_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS})

set(MAIN_LINK_LIBRARIES
    libfreenect2_sim 
    ${OpenCV_LIBRARIES}
    basic
    pthread
    )

# simulator ar include directories
set(AR_INCLUDE_DIRS
    ${OpenCV_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    simulator/cvdrones_sim/include)

set(AR_LINK_LIBRARIES
    ${BOOST_LIBRARIES}
    pthread
    cvdrones_sim basic)

# simulator visualizer include directories
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
