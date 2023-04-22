//
//  ray.h
//  ex3
//
//  Created by Begum Celik on 7.11.2020.
//

#ifndef ray_h
#define ray_h

class Ray {
    public:
        Ray() {}
    
        glm::dvec3 origin, direction;

        Ray(const glm::dvec3& origin, const glm::dvec3& direction)
            : origin(origin), direction(direction)
        {}


        glm::dvec3 at(double t) const
        {
            return origin + t * direction;
        }

    public:
        glm::dvec3 orig;
        glm::dvec3 dir;
};

#endif /* ray_h */
