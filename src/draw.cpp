#include "draw.h"

void draw::drawPersonVec(cv::Mat& img_data, const uint32_t frame_num, const std::vector<Person>& person_vec, const std::string type) {
    // Draw blobs
    for (std::vector<Person>::const_iterator it = person_vec.cbegin(); it != person_vec.cend(); it++) {
        if (it->isEnrolled()) {
            if (it->isLocked()) drawBlobVec(img_data, it->getId(), it->getLastBlobVec(), type);
            else if (it->getLastFrame() == frame_num) drawBlobVec(img_data, it->getId(), it->getBlobVec(frame_num), type);
        }
    }
    // Person counter
    std::string text = "People: " + std::to_string(Person::enroll_count);
    cv::putText(img_data, text, cv::Point2i(50, 50), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(255, 255, 255));

    // Play
    cv::imshow("CMK", img_data);
    if (behav::MANUAL_STEP) cv::waitKey(0);
    else cv::waitKey(behav::WAIT_TIME);
}

cv::Scalar draw::getRandColor(const uint32_t seed) {
    srand(seed);
    uint8_t red = rand() % 255;
    uint8_t green = rand() % 255;
    uint8_t blue = rand() % 255;
    cv::Scalar color(red, green, blue);
    return(color);
}

void draw::drawBlobVec(cv::Mat& img_data, const uint32_t id, const std::vector<Blob>& blob_vec, const std::string type) {
    cv::Scalar color = getRandColor(id);
    if (!type.compare("rect")) drawVecRect(img_data, id, color, blob_vec);
    else if (!type.compare("ellipse")) drawVecEllipse(img_data, id, color, blob_vec);
}

void draw::drawVecRect(cv::Mat& img_data, const uint32_t id, const cv::Scalar color, const std::vector<Blob>& blob_vec) {
    for (std::vector<Blob>::const_iterator it = blob_vec.cbegin(); it != blob_vec.cend(); it++) {
        drawText(img_data, id, color, it->getRect());
        drawBlob(img_data, color, it->getRect());
    }
}

void draw::drawVecEllipse(cv::Mat& img_data, const uint32_t id, const cv::Scalar color, const std::vector<Blob>& blob_vec) {
    for (std::vector<Blob>::const_iterator it = blob_vec.cbegin(); it != blob_vec.cend(); it++) {
        drawText(img_data, id, color, it->getEllipse());
        drawBlob(img_data, color, it->getEllipse());
    }
}

void draw::drawText(cv::Mat& img_data, const uint32_t id, const cv::Scalar color, const struct data::rect rect) {
    cv::putText(img_data, std::to_string(id), cv::Point2i(rect.xmin, rect.ymin - behav::TXT_OFFSET), cv::FONT_HERSHEY_SIMPLEX, 0.75, color);
}

void draw::drawText(cv::Mat& img_data, const uint32_t id, const cv::Scalar color, const struct data::ellipse ellipse) {
    cv::putText(img_data, std::to_string(id), cv::Point2i(ellipse.xc, ellipse.yc - ellipse.hy - behav::TXT_OFFSET), cv::FONT_HERSHEY_SIMPLEX, 0.75, color);
}

void draw::drawBlob(cv::Mat& img_data, const cv::Scalar color, const struct data::rect rect) {
    cv::rectangle(img_data, cv::Point2i(rect.xmin, rect.ymin), cv::Point2i(rect.xmax, rect.ymax), color, 2);
}

void draw::drawBlob(cv::Mat& img_data, const cv::Scalar color, const struct data::ellipse ellipse) {
    cv::ellipse(img_data, cv::Point2f(ellipse.xc, ellipse.yc), cv::Size2f(ellipse.hx, ellipse.hy), 0, 0, 360, color, 2);
}
