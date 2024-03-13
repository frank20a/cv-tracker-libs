#pragma once

#define _USE_MATH_DEFINES

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>


struct Bead {
    cv::Point2f center;
    std::vector<float> scores;
    std::vector<cv::Point> contour;
    float area, weighted_score = 0.0f;
};


class BeadDetector {
public:
    BeadDetector() = delete;
    BeadDetector(
        cv::Mat roi,
        std::vector<float> score_weights,
        uint8_t min_v = 70,
        uint8_t max_v = 110,
        uint8_t blur_radius = 5,
        uint8_t mask_dilate_radius = 7,
        uint8_t detection_erosion_radius = 3,
        unsigned int bead_min_area = 80,
        unsigned int bead_max_area = 350,
        unsigned int bead_nominal_area = 150
    ) : roi(roi), 
        s_w(score_weights), 
        min_hsv(0, 0, min_v), 
        max_hsv(255, 255, max_v),
        b_r(
            blur_radius, 
            blur_radius
        ),
        dilate_element(
            cv::getStructuringElement(
                cv::MORPH_ELLIPSE,
                cv::Size(
                    mask_dilate_radius, 
                    mask_dilate_radius
                )
            )
        ),
        erode_element(
            cv::getStructuringElement(
                cv::MORPH_ELLIPSE,
                cv::Size(
                    detection_erosion_radius, 
                    detection_erosion_radius
                )
            )
        ),
        b_a_min(bead_min_area),
        b_a_max(bead_max_area),
        b_a_nom(bead_nominal_area)
    {};
    void find(cv::Mat frame, std::vector<Bead> &beads);

private:
    cv::Mat roi;
    cv::Mat dilate_element, erode_element;
    std::vector<float> s_w;
    cv::Size b_r;
    unsigned int b_a_min, b_a_max, b_a_nom;
    cv::Scalar min_hsv, max_hsv;
};