#include "main.h"

int main(int argc, char* argv[]) {

    // Handle command line arguments
    /*************************************************************************/
    common::handleArgs(argc, argv);
    /*************************************************************************/

    // Initialize
    /*************************************************************************/
    cv::VideoCapture vid_reader(behav::INPUT_VID);
    std::vector<std::vector<Blob>> blob_stack;
    std::vector<Person> person_vec;
    Benchmark benchmark;
    float time_sum = 0.0f;
    /*************************************************************************/

    // Neural network setup
    /*************************************************************************/
    InferenceEngine::InferRequest infer_req;
    uint16_t net_width = 0, net_height = 0;
    neural::setup(infer_req, net_width, net_height);
    /*************************************************************************/

    for(uint32_t frame_num = 1;;frame_num++) {
        // Benchmarking
        /*************************************************************************/
        benchmark.start();
        /*************************************************************************/

        // Capture frame
        /*************************************************************************/
        cv::Mat frame_data;
        if (!common::readFrame(vid_reader, frame_data)) break;
        if (frame::isEnd(frame_num)) break;
        if (!frame::isStart(frame_num)) continue;
        std::printf("[INFO]: frame number = %u\n", frame_num);
        /*************************************************************************/

        // Detect all targets
        /*************************************************************************/
        std::vector<Blob> blob_vec;
        if (frame::isCNNActive(frame_num)) neural::detect(infer_req, frame_data, net_width, net_height, blob_vec);
        /*************************************************************************/

        // Assign ID
        /*************************************************************************/
        cmk::assignID(blob_vec, blob_stack, frame_num, person_vec);
        common::FIFO(blob_vec, blob_stack);
        /*************************************************************************/

        // CMK track
        /*************************************************************************/
        if (behav::CMK_ON) cmk::CMK(frame_data, frame_num, person_vec);
        /*************************************************************************/

        // Show results
        /*************************************************************************/
        draw::drawPersonVec(frame_data, frame_num, person_vec, "rect");
        std::printf("[INFO]: people count: %u\n", Person::enroll_count);
        /*************************************************************************/

        // Benchmarking
        /*************************************************************************/
        float frame_time = benchmark.end();
        time_sum += frame_time;
        float frame_time_avg = time_sum / frame_num;
        std::printf("[INFO]: frame time: %.2f ms\n", frame_time);
        std::printf("[INFO]: average frame time: %.2f ms\n", frame_time_avg);
        /*************************************************************************/
    }
    std::printf("Program finished\n");
    return(0);
}
