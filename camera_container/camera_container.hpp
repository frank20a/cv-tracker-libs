#pragma once

#include <opencv2/core.hpp>


class CameraContainer {
public:
    virtual bool begin() = 0;
    virtual bool end() = 0;
    virtual bool get_frame(cv::Mat &image) = 0;
};