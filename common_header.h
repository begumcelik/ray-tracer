//
//  common_header.h
//  ex3
//
//  Created by Begum Celik on 7.11.2020.
//

#ifndef common_header_h
#define common_header_h

#include <cmath>
#include <limits>
#include <memory>


#include "ray.h"

// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}

// Random double number generator between min and max values using drand[0,1]
double random_double(double min, double max)
{
    return min + (max-min) * random_double();
}

double length(glm::dvec3 vec){
    double length= vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
    return length;
    
}

// Random vector3 generator using random double
glm::dvec3 random(double min, double max)
{
    return glm::dvec3(random_double(min, max), random_double(min,max), random_double(min, max));
}

glm::dvec3 random_in_unit_sphere()
{
       while (true) {
            auto p = random(-1,1);
            if (length(p) >= 1) continue;
            return p;
    }
}

glm::dvec3 random_unit_vector(){
    return glm::normalize(random_in_unit_sphere());
}

bool near_zero(glm::dvec3 a)  {
     // Return true if the vector is close to zero in all dimensions.
     const auto s = 1e-8;
     return (fabs(a.x) < s) && (fabs(a.y) < s) && (fabs(a.z) < s);
 }

glm::dvec3 reflect(const glm::dvec3& v, glm::dvec3& n){
    return v- 2.0*glm::dot(v,n)*n;
}

glm::dvec3 refract(const glm::dvec3& uv, const glm::dvec3& n, double etai_over_etat){
    auto cos_theta = fmin(glm::dot(-1.0*uv, n), 1.0);
    glm::dvec3 r_out_perp = etai_over_etat * (uv + cos_theta*n);
    glm::dvec3 r_out_parallel = -1.0 * sqrt(fabs(1.0 - length(r_out_perp))) * n;
    return r_out_perp + r_out_parallel;
}
// Common Headers

#endif /* common_header_h */
