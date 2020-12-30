#include "main.h"

int main() {

    // Initialize
    cv::VideoCapture vid_reader(behav::INPUT_VID);
    std::vector<std::vector<Blob>> blob_stack;
    std::vector<Person> person_vec;

    // Neural network setup
    /*************************************************************************/
    InferenceEngine::InferRequest infer_req;
    uint16_t net_width = 0, net_height = 0;
    neural::setup(infer_req, net_width, net_height);
    /*************************************************************************/

    for(uint32_t frame_num = 1;;frame_num++) {
        // Capture frame
        /*************************************************************************/
        cv::Mat frame_data;
        if (!common::readFrame(vid_reader, frame_data)) break;
        if (frame::isEnd(frame_num)) break;
        if (!frame::isStart(frame_num)) continue;
        std::cout << "frame_num: " << frame_num << std::endl;
        /*************************************************************************/

        // Detect all targets
        /*************************************************************************/
        // TODO: handle missing targets from CNN that were tracked by CMK
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
        cmk::CMK(frame_data, frame_num, person_vec);
        /*************************************************************************/

        // Show results
        /*************************************************************************/
        draw::drawPersonVec(frame_data, frame_num, person_vec, "rect");
        /*************************************************************************/
    }
    std::cout << "Program finished." << std::endl;
    return(0);
}
