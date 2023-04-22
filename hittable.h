//
//  hittable.h
//  ex3
//
//  Created by Begum Celik on 7.11.2020.
//

#ifndef hittable_h
#define hittable_h

#include "common_header.h"

#include "aabb.h"

class material;


struct hit_record {
    glm::dvec3 p;
    glm::dvec3 normal;
    shared_ptr<material> mat_ptr;
    double t;
    double u; //U,V surface coordinates of the ray-object hit point.
    double v;
    bool front_face;
    
    inline void set_face_normal(const Ray& r, const glm::dvec3& outward_normal) {
          front_face = dot(r.direction, outward_normal) < 0;
          normal = front_face ? outward_normal :-outward_normal;
      }
};

class hittable {
    public:
        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const = 0;
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const = 0;
};


#endif /* hittable_h */
