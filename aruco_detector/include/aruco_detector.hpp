#include <opencv2/core.hpp>
#include <opencv2/aruco.hpp>
#include <iostream>


bool readCameraParameters(std::string filename, cv::Mat& cameraMatrix, cv::Mat& distCoeffs) {
    cv::FileStorage fs(filename, cv::FileStorage::READ);
    if (!fs.isOpened()) {
        std::cout << "Failed to open camera_params.yml" << std::endl;
        return false;
    }

    fs["camera_matrix"] >> cameraMatrix;
    fs["distortion_coefficients"] >> distCoeffs;

    return true;
}


class ArucoDetector {
public:
    ArucoDetector(std::string camera_calib_file);
    void detect(
        const cv::Mat& image, 
        std::vector<int>& ids, 
        std::vector<std::vector<cv::Point2f>>& corners,
        bool draw_markers = false
    );
    void detect(
        const cv::Mat& image, 
        std::vector<int>& ids, 
        std::vector<std::vector<cv::Point2f>>& corners,
        float marker_size,
        std::vector<cv::Vec3d> &rvecs,
        std::vector<cv::Vec3d> &tvecs,
        bool draw_markers = false
    );

private:
    cv::Ptr<cv::aruco::DetectorParameters> detector_params;
    cv::Ptr<cv::aruco::Dictionary> dictionary;
    cv::Mat cameraMatrix, distCoeffs;
};