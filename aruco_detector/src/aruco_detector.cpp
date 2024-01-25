#include <aruco_detector.hpp>


ArucoDetector::ArucoDetector(std::string camera_calib_file) {
    detector_params = cv::aruco::DetectorParameters::create();
    dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);
    readCameraParameters(camera_calib_file, cameraMatrix, distCoeffs);
}


void ArucoDetector::detect(
    const cv::Mat& image, 
    std::vector<int>& ids, 
    std::vector<std::vector<cv::Point2f>>& corners,
    bool draw_markers
) {
    cv::aruco::detectMarkers(image, dictionary, corners, ids, detector_params);

    if (draw_markers) {
        cv::aruco::drawDetectedMarkers(image, corners, ids);
    }
}


void ArucoDetector::detect(
    const cv::Mat& image, 
    std::vector<int>& ids, 
    std::vector<std::vector<cv::Point2f>>& corners,
    float marker_size,
    std::vector<cv::Vec3d> &rvecs,
    std::vector<cv::Vec3d> &tvecs,
    bool draw_markers
) {
    detect(image, ids, corners, draw_markers);

    if (marker_size > 0) {
        cv::aruco::estimatePoseSingleMarkers(corners, marker_size, cameraMatrix, distCoeffs, rvecs, tvecs);
    }
}
