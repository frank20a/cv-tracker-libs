#include <aruco_detector.hpp>


const cv::Ptr<cv::aruco::DetectorParameters> ArucoDetector::detector_params = cv::aruco::DetectorParameters::create();
const cv::Ptr<cv::aruco::Dictionary> ArucoDetector::dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_100);


void ArucoDetector::detect_markers(
        const cv::Mat& image,
        std::map<const int, Marker>& detections,
        float marker_size,
        bool draw_markers
) {
    corners.clear();
    ids.clear();

    cv::aruco::detectMarkers(image, dictionary, corners, ids, detector_params, rejected, cameraMatrix, distCoeffs);
    
    if (marker_size > 0) {
        if (draw_markers) {
            cv::aruco::drawDetectedMarkers(image, corners, ids);
        }

        std::vector<cv::Vec3d> rvecs, tvecs;
        cv::aruco::estimatePoseSingleMarkers(corners, marker_size, cameraMatrix, distCoeffs, rvecs, tvecs);

        detections.clear();
        for(int i = 0; i < ids.size(); i++) {
            Marker m;
            m.id = ids[i];
            m.corners = corners[i];
            m.rvec = rvecs[i];
            m.tvec = tvecs[i];

            detections.insert(std::pair<int, Marker>(m.id, m));
        }
        // std::sort(detections.begin(), detections.end(), [](const Marker& a, const Marker& b) {
        //     return a.id < b.id;
        // });
        
    }
}

/**
 * @brief Use the previous ArUco detections to calculate the pose of a board
 * 
 * @param image The image to detect the board in (must be the same as the one used to detect the markers)
 * @param board The board to detect
 * @param detection The detected board output
 * 
 * @return int The number of markers found in the board
 * 
 * This function uses the previous ArUco detections to calculate the pose of a board.
 * The board is detected using the markers found in the previous detection.
 * The function returns the number of markers found in the board.
 * The input image must be the same as the one used to detect the markers.
 */
int ArucoDetector::detect_board(
    const cv::Mat& image,
    const cv::Ptr<cv::aruco::Board> board,
    Marker& detection
) {
    cv::aruco::refineDetectedMarkers(image, board, corners, ids, rejected, cameraMatrix, distCoeffs);

    detection.id = -1;
    return cv::aruco::estimatePoseBoard(corners, ids, board, cameraMatrix, distCoeffs, detection.rvec, detection.tvec);
}
