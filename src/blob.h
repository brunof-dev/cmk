#pragma once

// General modules
#include <cstdint>
#include <cstdio>
#include <string>

// Local modules
#include "behav.h"
#include "data.h"

class Blob {
    private:
        struct data::rect m_rect;
        struct data::ellipse m_ellipse;
        bool m_dummy;
        uint32_t m_id;

    public:
        // Constructors
        Blob();
        Blob(const struct data::rect rect);
        Blob(const struct data::ellipse ellipse);

        // Getters
        struct data::rect getRect(void) const;
        struct data::ellipse getEllipse(void) const;
        bool isDummy(void) const;
        int32_t getId(void) const;

        // Setters
        void setRect(const struct data::rect rect);
        void setEllipse(const struct data::ellipse ellipse);
        void setId(const int32_t id);

        // Operators
        bool operator==(const Blob& blob) const;
        bool operator!=(const Blob& blob) const;

        // General functions
        struct data::rect getRectFromEllipse(const struct data::ellipse ellipse);
        struct data::ellipse getEllipseFromRect(const struct data::rect rect);
        void updateAll(const struct data::rect rect);
        void updateAll(const struct data::ellipse ellipse);
        void print(std::string type) const;
};
