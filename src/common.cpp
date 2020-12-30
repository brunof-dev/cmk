#include "common.h"

// General functions
bool common::sanity(const cv::Mat& img_data, const float xc, const float yc, const float hx, const float hy) {
    bool rc = true;

    // Get frame dimensions
    uint16_t frame_width = img_data.cols, frame_height = img_data.rows;

    // Minimum/maximum coordinates
    float xmin = 0.0f, xmax = static_cast<float>(frame_width) - 1.0f, ymin = 0.0f, ymax = static_cast<float>(frame_height) - 1.0f;

    // Check coordinates
    if (((xc - hx) < xmin) || ((xc + hx) > xmax)) rc = false;
    else if (((yc - hy) < ymin) || ((yc + hy) > ymax)) rc = false;

    return(rc);
}

void common::fixCoord(const cv::Mat& img_data, float& xc, float& yc, float& hx, float& hy) {
    // Get frame dimensions
    uint16_t frame_width = img_data.cols, frame_height = img_data.rows;

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
