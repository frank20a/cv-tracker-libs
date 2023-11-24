#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <iostream>

class ROISelector {
public:
    ROISelector();
    cv::Mat get(cv::Mat &frame);

private:
    cv::Size resize;
    bool resize_flag = false;
    std::vector<cv::Point> roi;
    
    static void onMouse(int event, int x, int y, int, void* data);
};