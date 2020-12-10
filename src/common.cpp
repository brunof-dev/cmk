#include "common.h"

// General functions
void common::getFrameDim(const cv::Mat& img_data, uint16_t& frame_width, uint16_t& frame_height) {
    frame_width = img_data.cols;
    frame_height = img_data.rows;
}

bool common::sanity(const cv::Mat& img_data, const float xc, const float yc, const float hx, const float hy) {
    bool rc = true;

    // Get frame dimensions
    uint16_t frame_width, frame_height;
    getFrameDim(img_data, frame_width, frame_height);

    // Minimum/maximum coordinates
    float xmin = 0.0f, xmax = static_cast<float>(frame_width) - 1.0f, ymin = 0.0f, ymax = static_cast<float>(frame_height) - 1.0f;

    // Check coordinates
    if (((xc - hx) < xmin) || ((xc + hx) > xmax)) rc = false;
    else if (((yc - hy) < ymin) || ((yc + hy) > ymax)) rc = false;

    return(rc);
}

void common::fixCoord(const cv::Mat& img_data, float& xc, float& yc, float& hx, float& hy) {
    // Get frame dimensions
    uint16_t frame_width, frame_height;
    getFrameDim(img_data, frame_width, frame_height);

    // Minimum/maximum coordinates
    float xmin = 0.0f, xmax = static_cast<float>(frame_width) - 1.0f, ymin = 0.0f, ymax = static_cast<float>(frame_height) - 1.0f;

    // Fix center coordinates
    if (xc < xmin) xc = xmin;
    else if (xc > xmax) xc = xmax;
    if (yc < ymin) yc = ymin;
    else if (yc > ymax) yc = ymax;

    // Fix ellipse axis
    if ((xc - hx) < xmin) hx = xc;
    else if ((xc + hx) > xmax) hx = xmax - xc;
    if ((yc - hy) < ymin) hy = yc;
    else if ((yc + hy) > ymax) hy = ymax - yc;
}

cv::Mat common::getROI(const cv::Mat& img_data, float& xc, float& yc, float& hx, float& hy) {
    // Sanity check
    if (!sanity(img_data, xc, yc, hx, hy)) fixCoord(img_data, xc, yc, hx, hy);

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

bool common::readFrame(cv::VideoCapture& vid_reader, cv::Mat& img_data) {
    bool rc = vid_reader.read(img_data);
    if (rc == false) vid_reader.release();
    return(rc);
}

uint32_t common::distance(const struct data::rect a, const struct data::rect b) {
    // Coordinates of the center of the bounding boxes
    uint16_t a_xc = static_cast<uint16_t>((a.xmin + a.xmax) * 0.5);
    uint16_t a_yc = static_cast<uint16_t>((a.ymin + a.ymax) * 0.5);
    uint16_t b_xc = static_cast<uint16_t>((b.xmin + b.xmax) * 0.5);
    uint16_t b_yc = static_cast<uint16_t>((b.ymin + b.ymax) * 0.5);

    // Distance
    uint32_t dist = (a_xc - b_xc) * (a_xc - b_xc) + (a_yc - b_yc) * (a_yc - b_yc);
    return(dist);
}

// Drawing functions
void common::drawPersonVec(cv::Mat& img_data, const uint32_t frame_num, const std::vector<Person>& person_vec, const std::string type) {
    // Draw blobs
    for (std::vector<Person>::const_iterator it = person_vec.cbegin(); it != person_vec.cend(); it++) {
        if ((it->getLastFrame() == frame_num) && (it->isEnrolled())) drawBlobVec(img_data, it->getId(), it->getBlobVec(frame_num), type);
    }
    // Person counter
    std::string text = "People: " + std::to_string(Person::enroll_count);
    cv::putText(img_data, text, cv::Point2i(50, 50), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(255, 255, 255));

    // Play
    cv::imshow("CMK", img_data);
    cv::waitKey(1);
}

cv::Scalar common::getRandColor(const uint32_t seed) {
    srand(seed);
    uint8_t red = rand() % 255;
    uint8_t green = rand() % 255;
    uint8_t blue = rand() % 255;
    cv::Scalar color(red, green, blue);
    return(color);
}

void common::drawBlobVec(cv::Mat& img_data, const uint32_t id, const std::vector<Blob>& blob_vec, const std::string type) {
    cv::Scalar color = getRandColor(id);
    if (!type.compare("rect")) drawVecRect(img_data, id, color, blob_vec);
    else if (!type.compare("ellipse")) drawVecEllipse(img_data, id, color, blob_vec);
}

void common::drawVecRect(cv::Mat& img_data, const uint32_t id, const cv::Scalar color, const std::vector<Blob>& blob_vec) {
    for (std::vector<Blob>::const_iterator it = blob_vec.cbegin(); it != blob_vec.cend(); it++) {
        drawText(img_data, id, color, it->getRect());
        drawBlob(img_data, color, it->getRect());
    }
}

void common::drawVecEllipse(cv::Mat& img_data, const uint32_t id, const cv::Scalar color, const std::vector<Blob>& blob_vec) {
    for (std::vector<Blob>::const_iterator it = blob_vec.cbegin(); it != blob_vec.cend(); it++) {
        drawText(img_data, id, color, it->getEllipse());
        drawBlob(img_data, color, it->getEllipse());
    }
}

void common::drawText(cv::Mat& img_data, const uint32_t id, const cv::Scalar color, const struct data::rect rect) {
    cv::putText(img_data, std::to_string(id), cv::Point2i(rect.xmin, rect.ymin - behav::TXT_OFFSET), cv::FONT_HERSHEY_SIMPLEX, 0.75, color);
}

void common::drawText(cv::Mat& img_data, const uint32_t id, const cv::Scalar color, const struct data::ellipse ellipse) {
    cv::putText(img_data, std::to_string(id), cv::Point2i(ellipse.xc, ellipse.yc - ellipse.hy - behav::TXT_OFFSET), cv::FONT_HERSHEY_SIMPLEX, 0.75, color);
}

void common::drawBlob(cv::Mat& img_data, const cv::Scalar color, const struct data::rect rect) {
    cv::rectangle(img_data, cv::Point2i(rect.xmin, rect.ymin), cv::Point2i(rect.xmax, rect.ymax), color, 2);
}

void common::drawBlob(cv::Mat& img_data, const cv::Scalar color, const struct data::ellipse ellipse) {
    cv::ellipse(img_data, cv::Point2f(ellipse.xc, ellipse.yc), cv::Size2f(ellipse.hx, ellipse.hy), 0, 0, 360, color, 2);
}
