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
        if (frame_num >= behav::FRAME_STOP) break;
        if (frame_num < (behav::FRAME_START - 1)) continue;
        std::cout << "frame_num: " << frame_num << std::endl;
        /*************************************************************************/

        // Detect all targets
        /*************************************************************************/
        std::vector<Blob> blob_vec;
        neural::detect(infer_req, frame_data, net_width, net_height, blob_vec);
        /*************************************************************************/

        // Assign ID
        /*************************************************************************/
        cmk::assignID(blob_vec, blob_stack, frame_num, person_vec);
        common::FIFO(blob_vec, blob_stack);
        /*************************************************************************/

        // CMK split
        /*************************************************************************/
        /*************************************************************************/

        // CMK track
        /*************************************************************************/
        // if (((frame_num - behav::FRAME_START) % behav::FRAME_GAP) == 0) {
        //     for (std::vector<Person>::iterator it = person_vec.begin(); it != person_vec.end(); it++) {
        //         if ((it->getLastFrame() == frame_num) && (it->isSplit())) {
        //             cmk::track(frame_data, it->m_blob_data);
        //         }
        //     }
        // }
        /*************************************************************************/

        // Show results
        /*************************************************************************/
        common::drawPersonVec(frame_data, frame_num, person_vec, "rect");
        /*************************************************************************/
    }
    std::cout << "Program finished." << std::endl;
    return(0);
}
