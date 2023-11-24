#pragma once

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

#include <iostream>
#include <string>

#include "camera_container.hpp"


class OpenCVCameraContainer : public CameraContainer { 
public:
    OpenCVCameraContainer(int index) { cap = cv::VideoCapture(index); };
    OpenCVCameraContainer(std::string filepath) { cap = cv::VideoCapture(filepath); };
    ~OpenCVCameraContainer() { cap.release(); };

    bool begin();
    bool end();
    bool get_frame(cv::Mat &image);

private:
    cv::VideoCapture cap;
};
