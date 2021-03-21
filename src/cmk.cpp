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
        if ((best_dist < behav::MAX_NB_DIST) && (found == true)) {
            nb_index = it - blob_vec.cbegin();
        }
    }
    return(nb_index);
}

void cmk::assignID(std::vector<Blob>& blob_vec, const std::vector<std::vector<Blob>>& blob_stack, const uint32_t frame_num, std::vector<Person>& person_vec) {
    // Assign ID based solely on current/previous CNN results
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
                    // Erase really old people to avoid memory leak
                    if ((!kt->isLocked()) && (kt->isReallyOld(frame_num))) {
                        kt = person_vec.erase(kt);
                        kt--;
                        continue;
                    }
                    if (!kt->isEnrolled()) {
                        // Non-enrolled person
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

    // Assign ID based on previous CMK results for lost CNN blobs
    if (behav::RECOVER_ON) {
        for (std::vector<Person>::iterator it = person_vec.begin(); it != person_vec.end(); it++) {
            // Skip already splitted people
            if (it->isLocked()) continue;
            // Skip really old people
            if (it->isReallyOld(frame_num)) continue;
            // Skip non-enrolled people
            if (!it->isEnrolled()) continue;
            // Skip people whose neighbours were found
            if (it->getLastFrame() == frame_num) continue;
            // Skip people unlocked just a few frames ago (CMK split/collapse loop cooldown)
            if (!it->isNeverLocked()) {
                it->incrUnlockCnt();
                if (it->getUnlockCnt() < behav::UNLOCK_CNT) continue;
            }
            // Skip people whose blobs got stuck
            if (it->isLazy()) continue;
            // Enable CMK track
            msg::debug("Recover lost person: id = %d\n", it->getId());
            struct data::rect last_rect = it->m_blob_data.at(it->m_blob_data.size() - 1).m_blob.getRect();
            it->enableCMK(frame_num, last_rect);
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
                    msg::debug("Splitting pair (%d, %d)\n",  person.getId(), it->getId());
                    found = true;
                    break;
                }
            }
            if (found) break;
        }
        if (found) break;
    }
    // Split offending blobs
    if (found) {
        // Enable CMK track
        person.enableCMK(frame_num, rect_a);
        it->enableCMK(frame_num, rect_b);
    }
}

void cmk::collapse(const uint32_t frame_num, Person& person) {
    // Unlock person to allow CNN to add new blobs (also allows person to be removed)
    person.unlock();
    // Find best CMK blob
    std::vector<Blob> lock_blob_vec = person.getLockBlobVec(), current_blob_vec = person.getLastBlobVec();
    struct data::rect lock_rect_high = lock_blob_vec.at(0).getRect(), lock_rect_low = lock_blob_vec.at(1).getRect();
    struct data::rect current_rect_high = current_blob_vec.at(0).getRect(), current_rect_low = current_blob_vec.at(1).getRect();
    uint16_t dist_high = geometry::distance(lock_rect_high, current_rect_high);
    uint16_t dist_low = geometry::distance(lock_rect_low, current_rect_low);
    bool best_high = false, best_low = false;
    if (dist_high >= dist_low) best_high = true;
    else best_low = true;
    // Mark people whose blobs did not move enough
    if ((dist_high < behav::MIN_CMK_DIST) && (dist_low < behav::MIN_CMK_DIST)) {
        msg::debug("Lazy person: id = %d\n", person.getId());
        person.setLazy();
    }
    else {
        // Extrapolate best rectangle
        struct data::rect best_rect;
        if (best_high) {
            best_rect = current_rect_high;
            best_rect.height = best_rect.height * 2;
            best_rect.ymax = best_rect.ymin + best_rect.height;
        }
        else if (best_low) {
            best_rect = current_rect_low;
            best_rect.height = best_rect.height * 2;
            best_rect.ymin = best_rect.ymax - best_rect.height;
        }
        // Add best CMK blob to latest frame
        person.addBlobData(frame_num, Blob(best_rect));
    }
}

void cmk::CMK(const cv::Mat& img_data, const uint32_t frame_num, std::vector<Person>& person_vec) {
    for (std::vector<Person>::iterator it = person_vec.begin(); it != person_vec.end(); it++) {
        // Try to split blobs
        if ((!it->isLocked()) && (it->getLastFrame() == frame_num) && (it->isEnrolled()) && (frame::isSplitActive(frame_num))) {
            split(frame_num, *it, person_vec);
        }
        // CMK track
        if ((it->isLocked()) && (frame::isTrackActive(frame_num))) {
            if (it->getLockCnt() >= behav::LOCK_CNT) {
                // Collapse splitted people back to single blob state
                msg::debug("Collapse person: id = %d\n", it->getId());
                collapse(frame_num, *it);
            }
            else {
                // Track splitted blobs
                trackBlobDataVec(img_data, it->getLastFrame(), it->m_blob_data);
                it->incrLockCnt();
            }
        }
    }
}
