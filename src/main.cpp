#include "main.h"

int main(int argc, char* argv[]) {

    // Handle command line arguments
    /*************************************************************************/
    common::handleArgs(argc, argv);
    /*************************************************************************/

    // Initialize
    /*************************************************************************/
    cv::VideoCapture vid_reader(behav::INPUT_VID);
    cv::VideoWriter vid_writer("out.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 25, cv::Size(960, 540));
    std::vector<std::vector<Blob>> blob_stack;
    std::vector<Person> person_vec;
    Benchmark benchmark;
    float time_sum = 0.0f;
    uint32_t time_count = 0;
    /*************************************************************************/

    // Neural network setup
    /*************************************************************************/
    InferenceEngine::InferRequest infer_req;
    uint16_t net_width = 0, net_height = 0;
    neural::setup(infer_req, net_width, net_height);
    /*************************************************************************/

    // Inference loop
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
        msg::info("frame number: %u\n", frame_num);
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
        vid_writer.write(frame_data);
        msg::info("people count: %u\n", Person::enroll_count);
        /*************************************************************************/

        // Benchmarking
        /*************************************************************************/
        float frame_time = benchmark.end();
        time_sum += frame_time;
        time_count++;
        float frame_time_avg = time_sum / time_count;
        msg::info("frame time: %.2f ms\n", frame_time);
        msg::info("average frame time: %.2f ms\n", frame_time_avg);
        /*************************************************************************/
    }
    /*************************************************************************/

    // Release resources
    /*************************************************************************/
    vid_reader.release();
    vid_writer.release();
    /*************************************************************************/

    msg::info("Program finished\n");
    return(0);
}
