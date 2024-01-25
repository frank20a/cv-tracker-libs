# Add Libraries
add_library(camera_container INTERFACE)
add_library(opencv_camera STATIC
    opencv_camera/src/opencv_camera.cpp
)
add_library(spinnaker_camera STATIC
    spinnaker_camera/src/spinnaker_camera.cpp
    spinnaker_camera/src/spinsys.cpp
)
add_library(bead_tracker STATIC
    bead_tracker/src/detector.cpp
    bead_tracker/src/fps.cpp
    bead_tracker/src/tracker.cpp
    bead_tracker/src/roi.cpp
)
add_library(aruco_detector STATIC
    aruco_detector/src/aruco_detector.cpp
)

# Append CMake Module Path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Set Library Include Dirs
set(CameraContainer_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/camera_container)
set(OpenCVCamera_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/opencv_camera/include)
set(SpinnakerCamera_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/spinnaker_camera/include)
set(BeadTracker_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bead_tracker/include)
set(ArucoDetector_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/aruco_detector/include)

# Find Packages
find_package(OpenCV REQUIRED)
find_package(Spinnaker REQUIRED)

# Specify Include Directories
target_include_directories(camera_container INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/camera_container
)
target_include_directories(opencv_camera PUBLIC
    ${OpenCV_INCLUDE_DIRS}
    ${OpenCVCamera_INCLUDE_DIR}
)
target_include_directories(spinnaker_camera PUBLIC
    ${Spinnaker_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    ${SpinnakerCamera_INCLUDE_DIR}
)
target_include_directories(bead_tracker PUBLIC
    ${OpenCV_INCLUDE_DIRS}
    ${BeadTracker_INCLUDE_DIR}
)
target_include_directories(aruco_detector PUBLIC
    ${OpenCV_INCLUDE_DIRS}
    ${ArucoDetector_INCLUDE_DIR}
)

# Link Libraries
target_link_libraries(camera_container INTERFACE
    ${OpenCV_LIBRARIES}
)
target_link_libraries(opencv_camera PUBLIC
    ${OpenCV_LIBRARIES}
    camera_container
)
target_link_libraries(spinnaker_camera PUBLIC
    ${Spinnaker_LIBRARIES}
    ${OpenCV_LIBRARIES}
    camera_container
)
target_link_libraries(bead_tracker PUBLIC
    ${OpenCV_LIBRARIES}
)
target_link_libraries(aruco_detector PUBLIC
    ${OpenCV_LIBRARIES}
)