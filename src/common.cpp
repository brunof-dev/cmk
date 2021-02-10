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

void common::usage() {
    std::printf("\nSoftware to track people in a video\n\n");
    std::printf("-------------------------------------------------------------------------------------\n\n");
    std::printf("Usage:              ./tracker --video <file> --xml <file> --bin <file> [options]\n\n");
    std::printf("--video <file>      use <file> video sequence for tracking\n");
    std::printf("--xml <file>        use <file> for neural network description\n");
    std::printf("--bin <file>        use <file> for neural network weights\n\n");
    std::printf("Extended list of options:\n\n");
    std::printf("--manual            requires key stroke from user to go to the next frame. Default is automatic tracking\n");
    std::printf("--start <frame>     start tracking from a certain frame. Default is 1\n");
    std::printf("--stop <frame>      stop tracking when a certain frame is reached. Default is end of video sequence\n");
    std::printf("--cmk               use CMK method for tracking. Default is not to use CMK\n");
    std::printf("--recover           attempt to recover people lost by neural network using previous CMK results\n");
    std::printf("--kalman            use Kalman filtering on top of CMK and mean shift results. Default is not to use Kalman filtering\n");
    std::printf("--stack <size>      number of frames to consider in the past of the scene. Default is 10\n\n");
    std::printf("-------------------------------------------------------------------------------------\n\n");
    exit(0);
}

bool common::exists(std::string filename) {
    struct stat buf;
    return(stat(filename.c_str(), &buf) == 0);
}

void common::handleArgs(uint8_t argc, char* argv[]) {
    // Parse command line
    for (uint8_t i = 1; i < argc; i++) {
        std::string arg_str(argv[i]);
        if ((arg_str == "-h") || (arg_str == "--help")) {
            usage();
        }
        else if (arg_str == "--video") {
            if ((i + 1) >= argc) {
                std::printf("Missing argument for %s option. See program usage: ./tracker -h\n", arg_str.c_str());
                exit(1);
            }
            std::string filename(argv[i + 1]);
            if (!exists(filename)) {
                std::printf("No such file: %s\n", filename.c_str());
                exit(1);
            }
            behav::INPUT_VID = filename;
            i++;
        }
        else if (arg_str == "--xml") {
            if ((i + 1) >= argc) {
                std::printf("Missing argument for %s option. See program usage: ./tracker -h\n", arg_str.c_str());
                exit(1);
            }
            std::string filename(argv[i + 1]);
            if (!exists(filename)) {
                std::printf("No such file: %s\n", filename.c_str());
                exit(1);
            }
            behav::MODEL_XML = filename;
            i++;
        }
        else if (arg_str == "--bin") {
            if ((i + 1) >= argc) {
                std::printf("Missing argument for %s option. See program usage: ./tracker -h\n", arg_str.c_str());
                exit(1);
            }
            std::string filename(argv[i + 1]);
            if (!exists(filename)) {
                std::printf("No such file: %s\n", filename.c_str());
                exit(1);
            }
            behav::MODEL_BIN = filename;
            i++;
        }
        else if (arg_str == "--manual") {
            behav::MANUAL_STEP = true;
        }
        else if (arg_str == "--start") {
            if ((i + 1) >= argc) {
                std::printf("Missing argument for %s option. See program usage: ./tracker -h\n", arg_str.c_str());
                exit(1);
            }
            behav::FRAME_START = std::stoul(std::string(argv[i + 1]));
            i++;
        }
        else if (arg_str == "--stop") {
            if ((i + 1) >= argc) {
                std::printf("Missing argument for %s option. See program usage: ./tracker -h\n", arg_str.c_str());
                exit(1);
            }
            behav::FRAME_END = true;
            behav::FRAME_STOP = std::stoul(std::string(argv[i + 1]));
            i++;
        }
        else if (arg_str == "--cmk") {
            behav::CMK_ON = true;
        }
        else if (arg_str == "--recover") {
            behav::RECOVER_ON = true;
        }
        else if (arg_str == "--kalman") {
            behav::KALMAN_ON = true;
        }
        else if (arg_str == "--stack") {
            if ((i + 1) >= argc) {
                std::printf("Missing argument for %s option. See program usage: ./tracker -h\n", arg_str.c_str());
                exit(1);
            }
            behav::STACK_SIZE = std::stoul(std::string(argv[i + 1]));
            i++;
        }
        else {
            std::printf("Unregonized option: %s. See program usage: ./tracker -h\n", arg_str.c_str());
            exit(1);
        }
    }
    // Enforce mandatory arguments
    bool bad_args = false;
    if (behav::INPUT_VID == "dummy") {
        std::printf("Missing mandatory argument: --video <file>\n");
        bad_args = true;
    }
    if (behav::MODEL_XML == "dummy") {
        std::printf("Missing mandatory argument: --xml <file>\n");
        bad_args = true;
    }
    if (behav::MODEL_BIN == "dummy") {
        std::printf("Missing mandatory argument: --bin <file>\n");
        bad_args = true;
    }
    if (bad_args) {
        std::printf("See program usage: ./tracker -h\n");
        exit(1);
    }
}
