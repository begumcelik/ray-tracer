//
//  box.h
//  ex3
//
//  Created by Begum Celik on 9.11.2020.
//

#ifndef box_h
#define box_h


#include "common_header.h"

#include "rect.h"
#include "hittable_list.h"

class box : public hittable  {
    public:
        box() {}
        box(const glm::dvec3& p0, const glm::dvec3& p1, shared_ptr<material> ptr);

        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            output_box = aabb(box_min, box_max);
            return true;
        }

    public:
        glm::dvec3 box_min;
        glm::dvec3 box_max;
        hittable_list sides;
};

box::box(const glm::dvec3& p0, const glm::dvec3& p1, shared_ptr<material> ptr) {
    box_min = p0;
    box_max = p1;

    sides.add(make_shared<xy_rect>(p0.x, p1.x, p0.y, p1.y, p1.z, ptr));
    sides.add(make_shared<xy_rect>(p0.x, p1.x, p0.y, p1.y, p0.z, ptr));

    sides.add(make_shared<xz_rect>(p0.x, p1.x, p0.z, p1.z, p1.y, ptr));
    sides.add(make_shared<xz_rect>(p0.x, p1.x, p0.z, p1.z, p0.y, ptr));

    sides.add(make_shared<yz_rect>(p0.y, p1.y, p0.z, p1.z, p1.x, ptr));
    sides.add(make_shared<yz_rect>(p0.y, p1.y, p0.z, p1.z, p0.x, ptr));
}

bool box::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
    return sides.hit(r, t_min, t_max, rec);
}


#endif /* box_h */
