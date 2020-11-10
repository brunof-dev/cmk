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

void common::drawPersonVec(cv::Mat& img_data, const std::vector<Person>& person_vec, const std::string type) {
    // Draw
    for (std::vector<Person>::const_iterator it = person_vec.cbegin(); it != person_vec.cend(); it++) {
        drawBlobVec(img_data, it->getBlobVec(), type);
    }
    // Play
    cv::imshow("CMK", img_data);
    cv::waitKey(1);
}

void common::drawBlobVec(cv::Mat& img_data, const std::vector<Blob>& blob_vec, const std::string type) {
    // Draw
    if (!type.compare("rect")) drawBlobVecRect(img_data, blob_vec);
    else if (!type.compare("ellipse")) drawBlobVecEllipse(img_data, blob_vec);
}

void common::drawBlobVecRect(cv::Mat& img_data, const std::vector<Blob>& blob_vec) {
    for (std::vector<Blob>::const_iterator it = blob_vec.cbegin(); it != blob_vec.cend(); it++) drawBlob(img_data, it->getRect());
}

void common::drawBlobVecEllipse(cv::Mat& img_data, const std::vector<Blob>& blob_vec) {
    for (std::vector<Blob>::const_iterator it = blob_vec.cbegin(); it != blob_vec.cend(); it++) {
        drawBlob(img_data, it->getEllipse());
    }
}

void common::drawBlob(cv::Mat& img_data, const struct data::rect rect) {
    cv::rectangle(img_data, cv::Point2i(rect.xmin, rect.ymin), cv::Point2i(rect.xmax, rect.ymax), cv::Scalar(255, 255, 255), 2);
}

void common::drawBlob(cv::Mat& img_data, const struct data::ellipse ellipse) {
    cv::ellipse(img_data, cv::Point2f(ellipse.xc, ellipse.yc), cv::Size2f(ellipse.hx, ellipse.hy), 0, 0, 360, cv::Scalar(255, 255, 255), 2);
}
