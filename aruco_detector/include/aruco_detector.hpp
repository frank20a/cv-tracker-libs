#pragma once

#include <opencv2/core.hpp>
#include <opencv2/aruco.hpp>
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>


struct Marker {
    int id;
    std::vector<cv::Point2f> corners;
    cv::Vec3d rvec, tvec;
};


class ArucoDetector {
public:

    ArucoDetector() = delete;
    ArucoDetector(cv::Mat cameraMatrix, cv::Mat distCoeffs) : cameraMatrix(cameraMatrix), distCoeffs(distCoeffs) {}
    void detect_markers(
        const cv::Mat& image,
        std::map<const int, Marker>& detections,
        float marker_size,
        bool draw_markers = false
    );
    int detect_board(
        const cv::Mat& image,
        const cv::Ptr<cv::aruco::Board> board,
        Marker& detection
    );

private:
    static const cv::Ptr<cv::aruco::DetectorParameters> detector_params;
    static const cv::Ptr<cv::aruco::Dictionary> dictionary;
    std::vector<std::vector<cv::Point2f>> corners, rejected;
    std::vector<int> ids;
    cv::Mat cameraMatrix, distCoeffs;
};