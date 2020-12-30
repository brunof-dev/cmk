#include "cmk.h"

int32_t cmk::findNeighbour(const Blob blob, const std::vector<Blob>& blob_vec) {
    int32_t nb_index = behav::INVALID_INDEX;
    uint32_t best_dist;
    for (std::vector<Blob>::const_iterator it = blob_vec.cbegin(); it != blob_vec.cend(); it++) {
        // Distance between blobs
        uint32_t dist = geometry::distance(blob.getRect(), it->getRect());
        // Max person speed threshold
        bool found = false;
        if ((it == blob_vec.begin()) || (dist < best_dist)) {
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

void cmk::trackBlobDataVec(const cv::Mat& img_data, const uint32_t frame_num, std::vector<BlobData>& blob_data_vec) {
    // BlobData loop
    for (std::vector<BlobData>::iterator it = blob_data_vec.begin(); it != blob_data_vec.end(); it++) {
        // Skip old blobs
        if (it->getFrameNum() != frame_num) continue;

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

std::vector<Blob> cmk::getNewBlobVec(const struct data::rect orig_rect) {
    std::vector<Blob> new_blob_vec;
    struct data::rect rect_high, rect_low;
    // Higher blob
    rect_high.xmin = orig_rect.xmin;
    rect_high.xmax = orig_rect.xmax;
    rect_high.ymin = orig_rect.ymin;
    rect_high.ymax = orig_rect.ymin + 0.5 * orig_rect.height;
    rect_high.width = geometry::getLength(rect_high.xmin, rect_high.xmax);
    rect_high.height = geometry::getLength(rect_high.ymin, rect_high.ymax);
    Blob blob_high(rect_high);
    // Lower blob
    rect_low.xmin = orig_rect.xmin;
    rect_low.xmax = orig_rect.xmax;
    rect_low.ymin = orig_rect.ymin + 0.5 * orig_rect.height;
    rect_low.ymax = orig_rect.ymax;
    rect_low.width = geometry::getLength(rect_low.xmin, rect_low.xmax);
    rect_low.height = geometry::getLength(rect_low.ymin, rect_low.ymax);
    Blob blob_low(rect_low);
    // Add blobs
    new_blob_vec.push_back(blob_high);
    new_blob_vec.push_back(blob_low);
    return(new_blob_vec);
}

void cmk::split(const uint32_t frame_num, Person& person, std::vector<Person>& person_vec) {
    // Find split condition
    bool found = false;
    struct data::rect rect_a, rect_b;
    std::vector<Person>::iterator it = person_vec.begin();
    for (; it != person_vec.end(); it++) {
        // Skip already splitted people
        if (it->isLocked()) continue;
        // Skip itself
        if (person.getId() == it->getId()) continue;
        // Skip old people
        if (it->getLastFrame() != frame_num) continue;
        // Skip non-enrolled people
        if (!it->isEnrolled()) continue;
        // Check IOU for all BlobData pairs between different people
        for (std::vector<BlobData>::const_iterator jt = person.m_blob_data.cbegin(); jt != person.m_blob_data.cend(); jt++) {
            // Skip old blobs
            if (jt->getFrameNum() != frame_num) continue;
            rect_a = jt->m_blob.getRect();
            for (std::vector<BlobData>::const_iterator kt = it->m_blob_data.cbegin(); kt != it->m_blob_data.cend(); kt++) {
                // Skip old blobs
                if (kt->getFrameNum() != frame_num) continue;
                rect_b = kt->m_blob.getRect();
                if (geometry::IOU(rect_a, rect_b) > behav::IOU_SPLIT) {
                    std::cout << "Splitting pair (" << person.getId() << ", " << it->getId() << ")" << std::endl;
                    found = true;
                    break;
                }
            }
            if (found) break;
        }
        if (found) break;
    }
    // Split blobs
    if (found) {
        // Remove original blob
        person.removeLastBlobData();
        it->removeLastBlobData();
        // Create new blobs
        std::vector<Blob> blob_vec_a = getNewBlobVec(rect_a);
        std::vector<Blob> blob_vec_b = getNewBlobVec(rect_b);
        // Add new blobs
        person.addBlobData(frame_num, blob_vec_a);
        it->addBlobData(frame_num, blob_vec_b);
        // Store original blobs before CMK track
        person.setLockBlobVec(blob_vec_a);
        it->setLockBlobVec(blob_vec_b);
        // Lock people to prevent blobs from CNN to be added before CMK has done its job
        person.lock();
        it->lock();
    }
}

void cmk::collapse(const uint32_t frame_num, Person& person) {
    // Find best blob
    std::vector<Blob> lock_blob_vec = person.getLockBlobVec(), current_blob_vec = person.getLastBlobVec();
    struct data::rect best_rect;
    uint32_t best_dist;
    for (std::vector<Blob>::const_iterator it = lock_blob_vec.cbegin(); it != lock_blob_vec.cend(); it++) {
        struct data::rect rect_a = it->getRect();
        for (std::vector<Blob>::const_iterator jt = current_blob_vec.cbegin(); jt != current_blob_vec.cend(); jt++) {
            struct data::rect rect_b = jt->getRect();
            uint32_t dist = geometry::distance(rect_a, rect_b);
            if ((jt == current_blob_vec.cbegin()) || (dist > best_dist)) {
                best_dist = dist;
                best_rect = rect_b;
            }
        }
    }
    // Unlock person to allow CNN to add new blobs
    person.unlock();
    // Add best CMK blob to latest frame
    person.addBlobData(frame_num, Blob(best_rect));
}

void cmk::CMK(const cv::Mat& img_data, const uint32_t frame_num, std::vector<Person>& person_vec) {
    for (std::vector<Person>::iterator it = person_vec.begin(); it != person_vec.end(); it++) {
        // Erase really old people to avoid memory leak
        if (!it->isLocked() && frame::isReallyOld(frame_num, it->getLastFrame())) {
            person_vec.erase(it);
            it--;
            continue;
        }
        if (behav::CMK_ON) {
            // Try to split blobs
            if ((!it->isLocked()) && (it->getLastFrame() == frame_num) && (it->isEnrolled()) && (frame::isSplitActive(frame_num))) {
                split(frame_num, *it, person_vec);
            }
            // CMK track
            if ((it->isLocked()) && (frame::isTrackActive(frame_num))) {
                if (it->getLockCnt() >= behav::LOCK_CNT) {
                    // Collapse splitted people back to single blob state
                    std::cout << "collapse: id = " << it->getId() << std::endl;
                    collapse(frame_num, *it);
                }
                else {
                    // Track splitted blobs
                    std::cout << "trackBlobDataVec: id = " << it->getId() << std::endl;
                    trackBlobDataVec(img_data, it->getLastFrame(), it->m_blob_data);
                    it->incrLockCnt();
                }
            }
        }
    }
}
