#include "neural.h"

void neural::setup(InferenceEngine::InferRequest& infer_req, uint16_t& net_width, uint16_t& net_height) {
    // Load plugin for IE
    InferenceEngine::Core core;

    // Read IR
    InferenceEngine::CNNNetwork net = core.ReadNetwork(behav::MODEL_XML, behav::MODEL_BIN);

    // Prepare input blob
    InferenceEngine::InputsDataMap input_map = net.getInputsInfo();
    std::cout << "Network input\tPrecision\tLayout\tColor format" << std::endl;
    for (std::map<std::string, InferenceEngine::InputInfo::Ptr>::iterator it = input_map.begin(); it != input_map.end(); ++it) {
        // Get blob dimensions
        InferenceEngine::SizeVector input_blob_sz = it->second->getTensorDesc().getDims();
        net_height = input_blob_sz[2];
        net_width = input_blob_sz[3];

        // Set precision
        it->second->setPrecision(InferenceEngine::Precision::U8);

        // Dump blob info
        std::cout << it->first << "\t" << it->second->getPrecision().name() << "\t\t" << it->second->getLayout() << "\t"\
                  << it->second->getPreProcess().getColorFormat() << std::endl;
    }

    // Prepare output blob
    InferenceEngine::OutputsDataMap output_map = net.getOutputsInfo();
    std::cout << "Network output\tPrecision" << std::endl;
    for (std::map<std::string, InferenceEngine::DataPtr>::iterator it = output_map.begin(); it != output_map.end(); ++it) {
        // Dump blob info
        std::cout << it->first << "\t" << it->second->getPrecision().name() << std::endl;
    }

    // Load model to target device
    std::map<std::string, std::string> config = {{CONFIG_KEY(CPU_THROUGHPUT_STREAMS), "1"}};
    InferenceEngine::ExecutableNetwork exec_net = core.LoadNetwork(net, behav::DEVICE, config);

    // Inference request
    infer_req = exec_net.CreateInferRequest();
}

void neural::fillBlob(InferenceEngine::Blob::Ptr input_blob, const cv::Mat& img_data) {
    // Memory management
    InferenceEngine::MemoryBlob::Ptr mem_input_blob = InferenceEngine::as<InferenceEngine::MemoryBlob>(input_blob);
    InferenceEngine::LockedMemory<void> mem_input_blob_hold = mem_input_blob->wmap();
    uint8_t* input_blob_data = mem_input_blob_hold.as<uint8_t*>();

    // Fill blob
    uint16_t width = img_data.cols;
    uint16_t height = img_data.rows;
    uint8_t channels = img_data.channels();
    for (size_t c = 0; c < channels; c++) {
        for (size_t  h = 0; h < height; h++) {
            for (size_t w = 0; w < width; w++) {
                input_blob_data[c * width * height + h * width + w] = img_data.at<cv::Vec3b>(h, w)[c];
            }
        }
    }
}

void neural::getBlobVec(InferenceEngine::Blob::Ptr output_blob, const uint16_t orig_width, const uint16_t orig_height, std::vector<Blob>& blob_vec) {
    // Memory management
    InferenceEngine::MemoryBlob::Ptr mem_output_blob = InferenceEngine::as<InferenceEngine::MemoryBlob>(output_blob);
    InferenceEngine::LockedMemory<const void> mem_output_blob_hold = mem_output_blob->rmap();
    const float* output_blob_data = mem_output_blob_hold.as<const float*>();

    // Get blob dimensions
    std::vector<size_t> output_blob_sz = output_blob->getTensorDesc().getDims();
    const size_t prop_num = output_blob_sz.at(2);
    const size_t prop_len  = output_blob_sz.at(3);

    // Iterate through proposals
    for (size_t i = 0; i < prop_num; i++) {
        // Parse info
        size_t index = i * prop_len;
        const int16_t img_id = static_cast<int16_t>(output_blob_data[index]);
        if (img_id == behav::END_CODE) break;
        const uint16_t class_id = static_cast<uint16_t>(output_blob_data[index + 1]);
        if (class_id == behav::HUMAN) {
            const float conf = output_blob_data[index + 2];
            if (conf > behav::CONF_LEVEL) {
                struct data::rect rect;
                rect.conf = conf;
                rect.xmin = static_cast<uint16_t>(output_blob_data[index + 3] * orig_width);
                rect.ymin = static_cast<uint16_t>(output_blob_data[index + 4] * orig_height);
                rect.xmax = static_cast<uint16_t>(output_blob_data[index + 5] * orig_width);
                rect.ymax = static_cast<uint16_t>(output_blob_data[index + 6] * orig_height);
                Blob blob(rect);
                blob_vec.push_back(blob);
            }
        }
    }
}

void neural::nonMaxSup(std::vector<Blob>& blob_vec) {
    for (std::vector<Blob>::iterator it = blob_vec.begin(); it != blob_vec.end(); it++) {
        // Big box suppression
        struct data::rect rect_a = it->getRect();
        uint16_t width = rect_a.xmax - rect_a.xmin + 1;
        uint16_t height = rect_a.ymax - rect_a.ymin + 1;
        if ((width > behav::MAX_WIDTH) || (height > behav::MAX_HEIGHT)) {
            it = blob_vec.erase(it);
            it--;
        }
        else {
            for (std::vector<Blob>::iterator jt = blob_vec.begin(); jt != blob_vec.end(); jt++) {
                // Non-max suppression
                struct data::rect rect_b = jt->getRect();
                if (common::isInner(rect_b, rect_a)) {
                    // Rectangle B is inside rectangle A
                    jt = blob_vec.erase(jt);
                    jt--;
                }
                else {
                    float iou = common::IOU(rect_a, rect_b);
                    if ((iou > behav::IOU_SUP) && (rect_a.conf > rect_b.conf)) {
                        // IOU between rectangles is high enough and rectangle B has lower confidence level
                        jt = blob_vec.erase(jt);
                        jt--;
                    }
                }
            }
        }
    }
}

void neural::detect(InferenceEngine::InferRequest& infer_req, const cv::Mat& img_data, const uint16_t net_width, const uint16_t net_height, std::vector<Blob>& blob_vec) {
    // Pre-process frame
    cv::Mat net_data;
    cv::resize(img_data, net_data, cv::Size(net_width, net_height));

    // Fill network input
    InferenceEngine::Blob::Ptr input_blob = infer_req.GetBlob(behav::INPUT_BLOB);
    fillBlob(input_blob, net_data);

    // Infer
    infer_req.Infer();

    // Filter bounding boxes
    InferenceEngine::Blob::Ptr output_blob = infer_req.GetBlob(behav::OUTPUT_BLOB);
    uint16_t frame_width, frame_height;
    common::getFrameDim(img_data, frame_width, frame_height);
    getBlobVec(output_blob, frame_width, frame_height, blob_vec);
    nonMaxSup(blob_vec);
}
