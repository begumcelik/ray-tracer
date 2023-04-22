//
//  aabb.h
//  ex3
//
//  Created by Begum Celik on 8.11.2020.
//

#ifndef aabb_h
#define aabb_h

#include "common_header.h"

class aabb {
    public:
        aabb() {}
        aabb(const glm::dvec3& a, const glm::dvec3& b) { minimum = a; maximum = b;}

        glm::dvec3 min() const {return minimum; }
        glm::dvec3 max() const {return maximum; }

        inline bool hit(const Ray& r, double t_min, double t_max) const {
            for (int a = 0; a < 3; a++) {
                auto invD = 1.0f / r.direction[a];
                auto t0 = (min()[a] - r.origin[a]) * invD;
                auto t1 = (max()[a] - r.origin[a]) * invD;
                if (invD < 0.0f)
                    std::swap(t0, t1);
                t_min = t0 > t_min ? t0 : t_min;
                t_max = t1 < t_max ? t1 : t_max;
                if (t_max <= t_min)
                    return false;
            }
            return true;
        }
           
      

        glm::dvec3 minimum;
        glm::dvec3 maximum;
};

aabb surrounding_box(aabb box0, aabb box1) {
    glm::dvec3 small(fmin(box0.min().x, box1.min().x),
                 fmin(box0.min().y, box1.min().y),
                 fmin(box0.min().z, box1.min().z));

    glm::dvec3 big(fmax(box0.max().x, box1.max().x),
               fmax(box0.max().y, box1.max().y),
               fmax(box0.max().z, box1.max().z));

    return aabb(small,big);
}
#endif /* aabb_h */
