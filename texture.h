//
//  texture.h
//  ex3
//
//  Created by Begum Celik on 8.11.2020.
//

#ifndef texture_h
#define texture_h

#include "common_header.h"

class texture {
    public:
        virtual glm::dvec3 value(double u, double v, const glm::dvec3& p) const = 0;
};

class solid_color : public texture {
    public:
        solid_color() {}
        solid_color(glm::dvec3 c) : color_value(c) {}

        solid_color(double red, double green, double blue)
          : solid_color(glm::dvec3(red,green,blue)) {}

        virtual glm::dvec3 value(double u, double v, const glm::dvec3& p) const override {
            return color_value;
        }

    private:
        glm::dvec3 color_value;
};

#endif /* texture_h */
