#include "main.h"

int main() {

    // Initialize
    cv::VideoCapture vid_reader(behav::INPUT_VID);
    cv::Mat frame_data, roi_data;
    Benchmark benchmark;
    bool rc = false, target_init = false, kalman_init = true;
    float xc = 884.0f, yc = 282.0f, hx = 42.0f, hy = 90.0f, simi = 0.0f, time_sum = 0.0f;
    std::vector<float> target_model(behav::BINS, 0.0f);

    for(uint32_t frame_num = 1;;frame_num++) {
        // benchmark.start();
        // Capture frame
        /*************************************************************************/
        rc = vid_reader.read(frame_data);
        if (rc == false) {
            vid_reader.release();
            break;
        }
        // std::cout << "frame_num: " << frame_num << std::endl;
        if (frame_num >= behav::FRAME_STOP) break;
        if (frame_num < (behav::FRAME_START - 1)) continue;
        /*************************************************************************/

        // Kalman filter (prediction)
        /*************************************************************************/
        if (target_init && (((frame_num - behav::FRAME_START) % behav::FRAME_GAP) == 0) && behav::KALMAN_ON) {
            if (kalman_init) {
                kalman::init(xc, yc, hx, hy);
                kalman_init = false;
            }
            // benchmark.start();
            kalman::predict(hx, hy);
            // benchmark.end("predict");
        }
        /*************************************************************************/

        // Mean shift (measure)
        /*************************************************************************/
        if (target_init && (((frame_num - behav::FRAME_START) % behav::FRAME_GAP) == 0)) {
            // benchmark.start();
            simi = mean_shift::meanShift(frame_data, xc, yc, hx, hy, target_model);
            // benchmark.end("mean_shift");
        }
        /*************************************************************************/

        // Kalman filter (update)
        /*************************************************************************/
        if (target_init && (((frame_num - behav::FRAME_START) % behav::FRAME_GAP) == 0) && behav::KALMAN_ON) {
            // benchmark.start();
            kalman::update(xc, yc, hx, hy, simi);
            // benchmark.end("update");
        }
        /*************************************************************************/

        // Initialize/Update target model
        /*************************************************************************/
        if ((((frame_num - behav::FRAME_START) % behav::FRAME_GAP) == 0) || (frame_num == (behav::FRAME_START - 1))) {
            float norm_factor = mean_shift::getNormFactor(hx, hy);
            cv::Mat roi_data = common::getROI(frame_data, xc, yc, hx, hy);
            target_model = mean_shift::getModel(roi_data, xc, yc, hx, hy, norm_factor);
            target_init = true;
        }
        /*************************************************************************/

        // Show results
        /*************************************************************************/
        common::draw(frame_data, xc, yc, hx, hy);
        /*************************************************************************/

        // Benchmark
        /*************************************************************************/
        // float frame_time = benchmark.end("frame");
        // time_sum += frame_time;
        // float frame_avg = time_sum / static_cast<float>(frame_num);
        // std::cout << "frame_avg: " <<  frame_avg << " ms" << std::endl;
        /*************************************************************************/
    }
    std::cout << "Program finished." << std::endl;
    return(0);
}
