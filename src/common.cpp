#include "common.h"

cv::Mat common::getROI(const cv::Mat& img_data, const float xc, const float yc, const float hx, const float hy) {
    // Black image
    cv::Mat mask(img_data.rows, img_data.cols, CV_8UC3, cv::Scalar(0, 0, 0));

    // Elipse mask
    cv::ellipse(mask, cv::Point2f(xc, yc), cv::Size2f(hx, hy), 0, 0, 360, cv::Scalar(255, 255, 255), -1);

    // Bitwise AND
    cv::Mat and_data;
    cv::bitwise_and(mask, img_data, and_data);

    // Crop ROI to reduce matrix size
    float xmin = xc - hx;
    float ymin = yc - hy;
    cv::Mat crop_data = and_data(cv::Rect2f(xmin, ymin, 2*hx, 2*hy));
    return(crop_data);
}

void common::draw(cv::Mat& img_data, const float xc, const float yc, const float hx, const float hy) {
    // Draw ellipse
    cv::ellipse(img_data, cv::Point2f(xc, yc), cv::Size2f(hx, hy), 0, 0, 360, cv::Scalar(255, 255, 255), 1);

    // Play
    cv::imshow("CMK", img_data);
    cv::waitKey(1);
}
