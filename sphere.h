//
//  sphere.h
//  ex3
//
//  Created by Begum Celik on 7.11.2020.
//

#ifndef sphere_h
#define sphere_h

#include "common_header.h"

#include "hittable.h"

class Sphere : public hittable
{
public:

    Sphere() {}
    
    Sphere(const glm::dvec3 c, double r, shared_ptr<material> m)
        : center(c), radius(r), mat_ptr(m) {};

    virtual bool hit(
                const Ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
    
public:
    glm::dvec3 center;
    double radius;
    shared_ptr<material> mat_ptr;
    
private:
        static void get_sphere_uv(const glm::dvec3& p, double& u, double& v) {
            // p: a given point on the sphere of radius one, centered at the origin.
            // u: returned value [0,1] of angle around the Y axis from X=-1.
            // v: returned value [0,1] of angle from Y=-1 to Y=+1.
            //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
            //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
            //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

            auto theta = acos(-p.y);
            auto phi = atan2(-p.z, p.x) + pi;

            u = phi / (2*pi);
            v = theta / pi;
        }
};

bool Sphere::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
    
    glm::dvec3 oc = r.origin - center;
    auto a = glm::dot(r.direction, r.direction);
    auto half_b= glm::dot(oc, r.direction);
    auto c= glm::dot(oc,oc) - radius * radius;
    
    
    auto discriminant = half_b*half_b - a*c;
    
    if (discriminant <0) {
        return false;
    }
    
    auto sqrtd = sqrt(discriminant);
    
    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b -sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if(root < t_min || t_max < root)
            return false;
    }
    
    rec.t=root;
    rec.p= r.at(rec.t);
    glm::dvec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    get_sphere_uv(outward_normal, rec.u, rec.v);
    rec.mat_ptr = mat_ptr;
    
    return true;
    
}

bool Sphere::bounding_box(double time0, double time1, aabb& output_box) const {
    output_box = aabb(
        center - glm::dvec3(radius, radius, radius),
        center + glm::dvec3(radius, radius, radius));
    return true;
}


#endif /* sphere_h */
