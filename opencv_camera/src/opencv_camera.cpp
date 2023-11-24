#include "opencv_camera.hpp"


bool OpenCVCameraContainer::begin() {
    if (!cap.isOpened()) {
        std::cout << "[OpenCVCameraContainer] Error opening video stream or file" << std::endl;
        return false;
    }

    return true;
}

bool OpenCVCameraContainer::get_frame(cv::Mat &frame) {
    cap.read(frame);

    if (frame.empty()) {
        std::cout << "[OpenCVCameraContainer] No frame captured" << std::endl;
        return false;
    }

    return true;
}

bool OpenCVCameraContainer::end() {
    cap.release();
    return true;
}
