#include "mean_shift.h"

float mean_shift::getSqDist(const float x, const float y) {
    return(x * x + y * y);
}

float mean_shift::getKE(const float x) {
    return(1.0f - x);
}

float mean_shift::getNormFactor(const float hx, const float hy) {
    // Get image size (relative to ROI)
    uint16_t rows = static_cast<uint16_t>(2.0f * hy);
    uint16_t cols = static_cast<uint16_t>(2.0f * hx);
    float xc = hx;
    float yc = hy;

    // Calculate weighted sum
    float weighted_sum = 0.0f;
    for (uint16_t i = 0; i < rows; i++) {
        for (uint16_t j = 0; j < cols; j++) {
            // Center coordinates by (xc, yc) and normalize by (hx, hy)
            float norm_x = (static_cast<float>(j) - xc) / hx;
            float norm_y = (yc - static_cast<float>(i)) / hy;

            // Calculate squared distance to center
            float dist = getSqDist(norm_x, norm_y);
            if (dist > 1.0f) continue;

            // Update weighted sum with Epanechnikov kernel profile
            weighted_sum += getKE(dist);
        }
    }

    // Calculate normalization factor
    float norm_factor = 1.0f / weighted_sum;
    return(norm_factor);
}

uint16_t mean_shift::getBin(const uint8_t blue, const uint8_t green, const uint8_t red) {
    // Number of bins per color channel (cubic root of behav::BINS)
    uint8_t channel_bins = 16;

    // Channel bin size
    float channel_bin_size = 256.0f / static_cast<float>(channel_bins);

    // Calculate to which bin each color channel belongs
    uint8_t blue_bin = blue / channel_bin_size;
    uint8_t green_bin = green / channel_bin_size;
    uint8_t red_bin = red / channel_bin_size;

    // Calculte general bin index
    uint16_t bin = blue_bin * channel_bins * channel_bins + green_bin * channel_bins + red_bin;
    return(bin);
}

std::vector<float> mean_shift::getModel(const cv::Mat& roi_data, const float xc, const float yc, const float hx, const float hy,
                                        const float norm_factor) {
    // Parse ROI dimensions
    uint8_t channels = roi_data.channels();
    uint16_t rows = roi_data.rows;
    uint16_t cols = roi_data.cols * channels;
    bool continuous = false;
    if (roi_data.isContinuous()) {
        cols *= rows;
        rows = 1;
        continuous = true;
    }

    // Get ellipse center coordinates (relative to ROI)
    float xc_roi = hx;
    float yc_roi = hy;

    // Iterate over image
    std::vector<float> model(behav::BINS, 0.0f);
    for (uint16_t i = 0; i < rows; i++) {
        const uint8_t* row_ptr = roi_data.ptr<uint8_t>(i);
        for (uint16_t j = 0; j < cols; j += channels) {
            // Get coordinates
            uint16_t real_row;
            if (continuous) real_row = j / (roi_data.cols * channels);
            else real_row = i;
            uint16_t real_col = j / channels;

            // Center coordinates by (xc, yc) and normalize by (hx, hy)
            float norm_x = (static_cast<float>(real_col) - xc_roi) / hx;
            float norm_y = (yc_roi - static_cast<float>(real_row)) / hy;

            // Calculate squared distance to center (skip masked out pixels)
            float dist = getSqDist(norm_x, norm_y);
            if (dist > 1.0f) continue;

            // Evaluate Epanechnikov kernel profile
            float kernel_dist = getKE(dist);

            // Get bin
            uint8_t blue = row_ptr[j];
            uint8_t green = row_ptr[j + 1];
            uint8_t red = row_ptr[j + 2];
            uint16_t bin = getBin(blue, green, red);

            // Update histogram
            model.at(bin) += kernel_dist;
        }
    }

    // Normalize histogram
    auto lambda = [norm_factor] (float i) -> float { return norm_factor * i; };
    std::transform(model.begin(), model.end(), model.begin(), lambda);
    return(model);
}

float mean_shift::getSimi(const std::vector<float>& a, const std::vector<float>& b) {
    float dist = 0.0f;
    uint16_t length = a.size();

    // Raw pointers to underlying array
    const float* a_ptr = a.data();
    const float* b_ptr = b.data();

    // Calculate Bhattacharrya coefficient
    for (int i = 0; i < length; i++) dist += std::sqrt(a_ptr[i] * b_ptr[i]);
    return(dist);
}

float mean_shift::getWeight(const std::vector <float>& target_model, const std::vector <float>& cand_model, const uint16_t bin) {
    float weight = 0.0f;
    if (cand_model.at(bin) == 0.0f) weight = 0.0f;
    else weight = std::sqrt(target_model.at(bin) / cand_model.at(bin));
    return(weight);
}

void mean_shift::getShiftVector(const cv::Mat& roi_data, const float xc, const float yc, const float hx, const float hy,
                                const std::vector <float>& target_model, const std::vector <float>& cand_model,
                                float& delta_x, float& delta_y) {
    // Parse ROI dimensions
    uint8_t channels = roi_data.channels();
    uint16_t rows = roi_data.rows;
    uint16_t cols = roi_data.cols * channels;
    bool continuous = false;
    if (roi_data.isContinuous()) {
        cols *= rows;
        rows = 1;
        continuous = true;
    }

    // Get ellipse center coordinates (relative to ROI)
    float xc_roi = hx;
    float yc_roi = hy;

    // Iterate over image
    float delta_x_sum = 0.0f, delta_y_sum = 0.0f, weight_sum = 0.0f;
    for (uint16_t i = 0; i < rows; i++) {
        const uint8_t* row_ptr = roi_data.ptr<uint8_t>(i);
        for (uint16_t j = 0; j < cols; j += channels) {
            // Get coordinates
            uint16_t real_row;
            if (continuous) real_row = j / (roi_data.cols * channels);
            else real_row = i;
            uint16_t real_col = j / channels;

            // Center coordinates by (xc, yc) and normalize by (hx, hy)
            float norm_x = (static_cast<float>(real_col) - xc_roi) / hx;
            float norm_y = (yc_roi - static_cast<float>(real_row)) / hy;

            // Calculate squared distance to center (skip masked out pixels)
            float dist = getSqDist(norm_x, norm_y);
            if (dist > 1.0f) continue;

            // Get bin
            uint8_t blue = row_ptr[j];
            uint8_t green = row_ptr[j + 1];
            uint8_t red = row_ptr[j + 2];
            uint16_t bin = getBin(blue, green, red);

            // Get pixel weight
            float weight = getWeight(target_model, cand_model, bin);

            // Update weighted sum
            delta_x_sum += norm_x * weight;
            delta_y_sum += norm_y * weight;
            weight_sum += weight;
        }
    }

    // Calculate shift vector scaled to real image coordinates
    delta_x = (delta_x_sum / weight_sum) * hx;
    delta_y = (delta_y_sum / weight_sum) * hy * -1.0f;
}

float mean_shift::updateCenter(const cv::Mat& img_data, float& xc, float& yc, float& hx, float& hy,
                               const std::vector<float>& target_model) {
    float old_simi = 0.0f, simi = 0.0f;
    const float norm_factor = mean_shift::getNormFactor(hx, hy);
    for (int iteration = 0; iteration < behav::ITERATION_MAX; iteration++) {
        // Get ROI
        cv::Mat roi_data = common::getROI(img_data, xc, yc, hx, hy);

        // Initialize/Update candidate model
        std::vector<float> cand_model = getModel(roi_data, xc, yc, hx, hy, norm_factor);

        // Evaluate Bhattacharrya coefficient
        simi = getSimi(target_model, cand_model);

        // Stop mean shift if Bhattacharrya coefficient decreases
        if (simi < old_simi) return(simi);

        // Get shift vector
        float delta_x = 0.0f, delta_y = 0.0f;
        getShiftVector(roi_data, xc, yc, hx, hy, target_model, cand_model, delta_x, delta_y);

        // Stop mean shift if increment is too low
        if (getSqDist(delta_x, delta_y) < behav::SHIFT_STEP_MIN) return(simi);

        // Update center of candidate ellipse
        // xc = 0.5f * xc + 0.5f * (xc + delta_x);
        // yc = 0.5f * yc + 0.5f * (yc + delta_y);
        xc += delta_x;
        yc += delta_y;

        // Keep track of Bhattacharrya coefficient
        old_simi = simi;
    }
    return(simi);
}

float mean_shift::meanShift(const cv::Mat& img_data, float& xc, float& yc, float& hx, float& hy,
                            const std::vector<float>& target_model) {
    std::vector<std::pair<float, float>> h_vec {std::make_pair(hx - behav::SCALE_STEP * hx, hy - behav::SCALE_STEP * hy),
                                                std::make_pair(hx, hy),
                                                std::make_pair(hx + behav::SCALE_STEP * hx, hy + behav::SCALE_STEP * hy)};
    float best_simi = -1.0f, hx_opt = hx, hy_opt = hy, xc_opt = 0.0f, yc_opt = 0.0f, old_xc = xc, old_yc = yc;
    for (std::vector<std::pair<float, float>>::iterator it = h_vec.begin(); it != h_vec.end(); it++) {
        // Update center of candidate ellipse
        float hx_try = it->first;
        float hy_try = it->second;
        float simi = updateCenter(img_data, xc, yc, hx_try, hy_try, target_model);

        // Retrieve best ellipse dimensions and coordinates
        if (simi > best_simi) {
            best_simi = simi;
            hx_opt = hx_try;
            hy_opt = hy_try;
            xc_opt = xc;
            yc_opt = yc;
        }

        // Restore center coordinates for next ellipse candidate
        xc = old_xc;
        yc = old_yc;
    }

    // Scale adaptation
    hx = behav::SCALE_ADAPT * hx_opt + (1 - behav::SCALE_ADAPT) * hx;
    hy = behav::SCALE_ADAPT * hy_opt + (1 - behav::SCALE_ADAPT) * hy;
    xc = xc_opt;
    yc = yc_opt;
    return(best_simi);
}
