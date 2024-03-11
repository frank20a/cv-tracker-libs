#include "roi.hpp"


ROISelector::ROISelector() {
    cv::namedWindow("Select ROI", cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback("Select ROI", ROISelector::onMouse, this);
}

cv::Mat ROISelector::get(cv::Mat &frame) {

    std::cout << "[ROI] Select ROI or press \'ESC\' to continue" << std::endl;
    for(;;) {
        cv::Mat frame_copy = frame.clone();

        if (cv::waitKey(10) == 27)
            break;
        
        for(int i = 0; i < roi.size(); i++) {
            cv::circle(frame_copy, roi[i], 2, cv::Scalar(0, 0, 255), 2);
            if (i > 0)
                cv::line(frame_copy, roi[i-1], roi[i], cv::Scalar(0, 0, 255), 2);
        }

        if(roi.size() > 2)
            cv::line(frame_copy, roi[0], roi[roi.size()-1], cv::Scalar(0, 0, 255), 2);

        cv::imshow("Select ROI", frame_copy);
    }

    cv::destroyAllWindows();

    cv::Mat mask;
    if(roi.size() > 2) {
        std::cout << "[ROI] ROI selected" << std::endl;
        mask = cv::Mat::zeros(frame.size(), CV_8UC1);
        cv::fillConvexPoly(mask, roi, cv::Scalar(255, 255, 255));
    } else {
        std::cout << "[ROI] WARNING: No ROI selected, using full frame" << std::endl;
        mask = 255 * cv::Mat::ones(frame.size(), CV_8UC1);
    }

    return mask;
}

void ROISelector::onMouse(int event, int x, int y, int, void* data) {
    ROISelector* roi = (ROISelector*) data;
    if (event == cv::EVENT_LBUTTONDOWN) {
        roi->roi.push_back(cv::Point(x, y));
    }
}
