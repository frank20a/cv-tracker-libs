#include "detector.hpp"


void BeadDetector::find(cv::Mat frame, std::vector<Bead> &beads) {
    cv::Mat hsv, grey, detections, mask, tmp;
    std::vector<std::vector<cv::Point>> contours;
    beads.clear();

    // Grayscale & HSV
    cv::cvtColor(frame, grey, cv::COLOR_BGR2GRAY);
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    // Create Thresholding Mask
    cv::GaussianBlur(grey, grey, b_r, 0, 0);
    cv::threshold(grey, mask, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);
    cv::dilate(mask, mask, dilate_element, cv::Point(-1, -1), 3);

    // // Apply ROI and Mask
    cv::bitwise_and(mask, roi, mask);
    hsv.copyTo(tmp, mask);
    tmp.copyTo(hsv);

    // cv::imshow("Mask", mask);

    // // HSV Filtering & Merge mask with ROI
    cv::inRange(hsv, min_hsv, max_hsv, detections);
    // cv::imshow("detections-pre", detections);
    cv::dilate(detections, detections, dilate_element, cv::Point(-1, -1), 1);
    cv::erode(detections, detections, erode_element, cv::Point(-1, -1), 2);
    cv::dilate(detections, detections, dilate_element, cv::Point(-1, -1), 1);

    // cv::imshow("detections-post", detections);

    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(detections, circles, cv::HOUGH_GRADIENT, 1, 15, 50, 10, 4, 18);

    // // Find contours and score detections
    cv::findContours(detections, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    for(auto contour : contours) {
        Bead bead;
        float r_min, r_max;

        cv::minEnclosingCircle(contour, bead.center, r_min);
        bead.area = cv::contourArea(contour);

        if (bead.area < 1e-3) continue;
        bead.scores.push_back(std::exp(-1 * std::abs(M_PI * r_min * r_min / bead.area - 1)));
        if (circles.size() == 0) {
            bead.scores.push_back(0.5f);
        } else {
            float min = 1e6;
            cv::Vec3f min_circle = circles[0];
            for (auto circle : circles) {
                if (std::abs(circle[0] - bead.center.x) < min) {
                    min = std::abs(circle[0] - bead.center.x);
                    min_circle = circle;
                }
            }
            bead.scores.push_back(std::exp(-1 * std::abs(M_PI * min_circle[2] * min_circle[2] / bead.area - 1)));
        }
        bead.scores.push_back(std::exp(-1 * std::max(b_a_min - bead.area, .0f) / b_a_min));
        bead.scores.push_back(std::exp(-1 * std::max(bead.area - b_a_max, .0f) / b_a_max));
        bead.scores.push_back(std::exp(-1 * std::abs(b_a_nom - bead.area) / b_a_nom));

        for(int i = 0; i < s_w.size(); i++) {
            if (bead.scores[i] < 0.15){
                bead.weighted_score = 0.0f;
                break;
            }
            // std::cout << s_w[i] << " " << bead.scores[i] << std::endl;
            bead.weighted_score += s_w[i] * bead.scores[i];
        }
        // std::cout << std::endl;
        // std::cout << s_w.size() << std::endl;

        bead.contour = contour;

        if (bead.weighted_score > 0.35f)
            beads.push_back(bead);
    }

    std::sort(beads.begin(), beads.end(), [](Bead a, Bead b) {
        return a.weighted_score > b.weighted_score;
    });

    // cv::imshow("Detections", detections);
}
