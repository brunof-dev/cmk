#include "main.h"

int main() {

    // Initialize
    cv::VideoCapture vid_reader(behav::INPUT_VID);
    cv::Mat frame_data, roi_data;
    Benchmark benchmark;
    std::vector<Person> person_vec;

    // Blobs
    // struct data::ellipse ellipse_a = {792.0f, 288.0f, 60.0f, 125.0f, 0.0f};
    // struct data::ellipse ellipse_b = {898.0f, 275.0f, 55.0f, 140.0f, 0.0f};
    // Blob blob_a(ellipse_a);
    // Blob blob_b(ellipse_b);
    // Person person_a(behav::FRAME_START, blob_a);
    // Person person_b(behav::FRAME_START, blob_b);
    // person_vec.push_back(person_a);
    // person_vec.push_back(person_b);

    // Neural network setup
    /*************************************************************************/
    InferenceEngine::InferRequest infer_req;
    uint16_t net_width = 0, net_height = 0;
    neural::setup(infer_req, net_width, net_height);
    /*************************************************************************/

    for(uint32_t frame_num = 1;;frame_num++) {
        // Capture frame
        /*************************************************************************/
        bool rc = vid_reader.read(frame_data);
        if (rc == false) {
            vid_reader.release();
            break;
        }
        if (frame_num >= behav::FRAME_STOP) break;
        if (frame_num < (behav::FRAME_START - 1)) continue;
        std::cout << "frame_num: " << frame_num << std::endl;
        /*************************************************************************/

        // Detect all targets
        /*************************************************************************/
        neural::detect(infer_req, frame_data, frame_num, net_width, net_height, person_vec);
        /*************************************************************************/

        // CMK split
        /*************************************************************************/
        /*************************************************************************/

        // Track all targets
        /*************************************************************************/
        if (((frame_num - behav::FRAME_START) % behav::FRAME_GAP) == 0) {
            // Person loop
            for (std::vector<Person>::iterator it = person_vec.begin(); it != person_vec.end(); it++) {
                // TODO: limit people storage to a fixed number of frames in the past
                // TODO: smart person ID scheme
                // Skip old people
                if (it->getLastFrame() != frame_num) continue;
                std::vector<BlobData>* blob_data_ptr = it->getData();
                // Blob loop
                for (std::vector<BlobData>::iterator jt = blob_data_ptr->begin(); jt != blob_data_ptr->end(); jt++) {
                    // Get handlers
                    Kalman* kalman_hdl = &(std::get<Person::KALMAN_INDEX>(*jt));
                    Blob* blob_hdl = &(std::get<Person::BLOB_INDEX>(*jt));
                    std::vector<float>* target_hdl = &(std::get<Person::TARGET_INDEX>(*jt));

                    // Get ellipse data from blob handler
                    struct data::ellipse ellipse = blob_hdl->getEllipse();
                    float xc = ellipse.xc, yc = ellipse.yc, hx = ellipse.hx, hy = ellipse.hy, simi = 0.0f;

                    if (blob_hdl->isInit()) {
                        // Kalman filter (prediction)
                        /*************************************************************************/
                        if (behav::KALMAN_ON) {
                            if (!kalman_hdl->isInit()) kalman_hdl->init(xc, yc, hx, hy);
                            kalman_hdl->predict(hx, hy);
                        }
                        /*************************************************************************/

                        // Mean shift (measure)
                        /*************************************************************************/
                        simi = mean_shift::meanShift(frame_data, xc, yc, hx, hy, *target_hdl);
                        /*************************************************************************/

                        // Kalman filter (update)
                        /*************************************************************************/
                        if (behav::KALMAN_ON) kalman_hdl->update(xc, yc, hx, hy, simi);
                        /*************************************************************************/
                    }


                    // Initialize/Update target model
                    /*************************************************************************/
                    float norm_factor = mean_shift::getNormFactor(hx, hy);
                    cv::Mat roi_data = common::getROI(frame_data, xc, yc, hx, hy);
                    *target_hdl = mean_shift::getModel(roi_data, xc, yc, hx, hy, norm_factor);
                    if (!blob_hdl->isInit()) blob_hdl->setInit();
                    /*************************************************************************/

                    // Update ellipse data of blob handler
                    ellipse.xc = xc, ellipse.yc = yc, ellipse.hx = hx, ellipse.hy = hy;
                    blob_hdl->updateAll(ellipse);
                }
            }
        }
        /*************************************************************************/

        // Show results
        /*************************************************************************/
        common::drawPersonVec(frame_data, frame_num, person_vec, "rect");
        /*************************************************************************/
    }
    std::cout << "Program finished." << std::endl;
    return(0);
}
