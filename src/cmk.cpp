#include "cmk.h"

int32_t cmk::findNeighbour(const Blob blob, const std::vector<Blob>& blob_vec) {
    int32_t nb_index = behav::INVALID_INDEX;
    uint32_t best_dist;
    for (std::vector<Blob>::const_iterator it = blob_vec.cbegin(); it != blob_vec.cend(); it++) {
        // Distance between blobs
        uint32_t dist = common::distance(blob.getRect(), it->getRect());
        // Max person speed threshold
        bool found = false;
        if (it == blob_vec.begin()) {
            best_dist = dist;
            found = true;
        }
        else if (dist < best_dist) {
            best_dist = dist;
            found = true;
        }
        if ((best_dist < behav::DIST) && (found == true)) {
            nb_index = it - blob_vec.cbegin();
        }
    }
    return(nb_index);
}

void cmk::assignID(std::vector<Blob>& blob_vec, const std::vector<std::vector<Blob>>& blob_stack, const uint32_t frame_num, std::vector<Person>& person_vec) {
    std::set<int32_t> nb_id_set;
    for (std::vector<Blob>::iterator it = blob_vec.begin(); it != blob_vec.end(); it++) {
        // Find neighbour blob in previous frames
        int32_t nb_index = behav::INVALID_INDEX;
        std::vector<std::vector<Blob>>::const_iterator jt = blob_stack.cbegin();
        for (; jt != blob_stack.cend(); jt++) {
            nb_index = findNeighbour(*it, *jt);
            if (nb_index != behav::INVALID_INDEX) break;
        }
        // Handle IDs
        if (nb_index == behav::INVALID_INDEX) {
            // New person
            Person person(frame_num, *it);
            person_vec.push_back(person);
            int32_t person_id = person.getId();
            it->setId(person_id);
        }
        else {
            // Old person
            int32_t nb_id = jt->at(nb_index).getId();
            if (nb_id_set.find(nb_id) != nb_id_set.end()) {
                // Duplicates
                Person person(frame_num, *it);
                person_vec.push_back(person);
                int32_t person_id = person.getId();
                it->setId(person_id);
            }
            else {
                // Non-duplicates
                nb_id_set.insert(nb_id);
                for (std::vector<Person>::iterator kt = person_vec.begin(); kt != person_vec.end(); kt++) {
                    if (!kt->isEnrolled()) {
                        // Not enrolled person
                        if (kt->getId() == nb_id) {
                            kt->addBlobData(frame_num, *it);
                            break;
                        }
                    }
                    else {
                        // Enrolled person
                        if (kt->getNbId() == nb_id) {
                            kt->addBlobData(frame_num, *it);
                            break;
                        }
                    }
                }
                it->setId(nb_id);
            }
        }
    }
}

void cmk::trackBlobDataVec(const cv::Mat& img_data, std::vector<BlobData>& blob_data_vec) {
    // BlobData loop
    for (std::vector<BlobData>::iterator it = blob_data_vec.begin(); it != blob_data_vec.end(); it++) {
        // Get handlers
        BlobData& blob_data_hdl = *it;
        Kalman& kalman_hdl = it->m_kalman;
        Blob& blob_hdl = it->m_blob;

        // Get ellipse data
        struct data::ellipse ellipse = blob_hdl.getEllipse();
        float xc = ellipse.xc, yc = ellipse.yc, hx = ellipse.hx, hy = ellipse.hy, simi = 0.0f;

        if (blob_data_hdl.isTargetInit()) {
            // Kalman filter (prediction)
            /*************************************************************************/
            if (behav::KALMAN_ON) {
                if (!kalman_hdl.isInit()) kalman_hdl.init(xc, yc, hx, hy);
                kalman_hdl.predict(hx, hy);
            }
            /*************************************************************************/

            // Mean shift (measure)
            /*************************************************************************/
            simi = mean_shift::meanShift(img_data, xc, yc, hx, hy, blob_data_hdl.getTargetModel());
            /*************************************************************************/

            // Kalman filter (update)
            /*************************************************************************/
            if (behav::KALMAN_ON) kalman_hdl.update(xc, yc, hx, hy, simi);
            /*************************************************************************/
        }

        // Initialize/Update target model
        /*************************************************************************/
        float norm_factor = mean_shift::getNormFactor(hx, hy);
        cv::Mat roi_data = common::getROI(img_data, xc, yc, hx, hy);
        blob_data_hdl.setTargetModel(mean_shift::getModel(roi_data, xc, yc, hx, hy, norm_factor));
        if (!blob_data_hdl.isTargetInit()) blob_data_hdl.setTargetInit();
        /*************************************************************************/

        // Update ellipse data of blob handler
        ellipse.xc = xc, ellipse.yc = yc, ellipse.hx = hx, ellipse.hy = hy;
        blob_hdl.updateAll(ellipse);
    }
}

// TODO: split the single blob person into multiple blobs for CMK tracking, based on behav::IOU_SPLIT between different people
// TODO: figure out when to collapse the splitted people back to single blob state
void cmk::split(Person& person, const std::vector<Person>& person_vec) {
    for (std::vector<Person>::const_iterator it = person_vec.cbegin(); it != person_vec.cend(); it++) {
    }
}

void cmk::CMK(const cv::Mat& img_data, const uint32_t frame_num, std::vector<Person>& person_vec) {
    for (std::vector<Person>::iterator it = person_vec.begin(); it != person_vec.end(); it++) {
        if (it->getLastFrame() == frame_num) {
            // split(*it, person_vec);
            if (it->isSplit()) {
                std::cout << "trackBlobDataVec: id = " << it->getId() << std::endl;
                trackBlobDataVec(img_data, it->m_blob_data);
            }
        }
        else if ((frame_num - it->getLastFrame()) > behav::STACK_SIZE) {
            // Erasing really old people to avoid memory leak
            person_vec.erase(it);
            it--;
        }
    }
}
