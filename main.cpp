//
//  main.cpp
//  ex3
//
//  Created by Begum Celik on 31.10.2020.
//

#include <iostream>
#include <vector>
#include "glm/glm.hpp"



#include "common_header.h"

#include "IO.h"
#include "PixelBuffer.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "rect.h"
#include "box.h"


glm::dvec3 ray_color (const Ray& r,const glm::dvec3& background, const hittable& world, int depth){
    
    hit_record rec;
    
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return glm::dvec3(0,0,0);
    
    // If the ray hits nothing, return the background color.
       if (!world.hit(r, 0.001, infinity, rec))
           return background;

       Ray scattered;
       glm::dvec3 attenuation;
       glm::dvec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

       if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
           return emitted;

       return emitted + attenuation * ray_color(scattered, background, world, depth-1);

}


struct Camera
{
    glm::dvec3 position, left_bottom, horizontal, vertical;
    double focal_length;

    Camera(const glm::dvec3& position, const glm::dvec3& target, const PixelBuffer& pixel_buffer)
        : position(position), focal_length(1)
    {
        glm::dvec3 up(0, 1, 0);     // up is pointing through +x, constant for now, it should be changed to tilt the camera, this vector is perpendicular to camera direction
        auto forward = glm::normalize(target - position);    // direction from eye to target point
        auto right = glm::normalize(glm::cross(forward, up));      // cross product is perpendicular to those 2 vectors, remember the right hand rule
        up = glm::normalize(glm::cross(forward, -right));       // direction of up vector

        horizontal = right * (double(pixel_buffer.dimensions.x) / double(pixel_buffer.dimensions.y));   // height/width ratio of the raster image in the direction of right vector
        vertical = up * 1.;
        left_bottom = position + forward * focal_length - horizontal * 0.5 - vertical * 0.5;
    }

    // Mappping raster points into 3D world points
    glm::dvec3 raster_to_world(const glm::dvec2& r)
    {
        return left_bottom + r.x * horizontal + r.y * vertical;
    }
    
    Ray get_ray(double u, double v)const
    {
        return Ray(position, left_bottom + u*horizontal + v*vertical- position);
    }
};


glm::dvec3 colored_normals(const Ray& ray,hittable_list world){
    
    
    hit_record rec;
    
    
    if (!world.hit(ray, 0.001, infinity, rec)) {
        
        // Sky Background
        glm::dvec3 unit_direction= glm::normalize(ray.direction);
        double t = 0.9 * (unit_direction.y + 1.0);
        return (1.0 - t) + t * glm::dvec3(0.5, 0.7, 1.0);
    }
    
    glm::dvec3 N = glm::normalize(rec.normal); // unit vector of normals whose components are between -1 and 1
    return  glm::normalize((glm::dvec3(N.x+1, N.y+1, N.z+1))); // map each component of N to the interval 0 to 1
    
    
}

hittable_list cornell_box() {
    hittable_list objects;

    auto red   = make_shared<metal>(glm::dvec3(.25, .05, .15),1.0);
    auto white = make_shared<metal>(glm::dvec3(.73, .73, .73),1.0);
    auto green = make_shared<metal>(glm::dvec3(.12, .25, .35),1.0);
    auto light = make_shared<diffuse_light>(glm::dvec3(15, 15, 15));

    objects.add(make_shared<yz_rect>(-300, 880, -300, 880, 880, green));
    objects.add(make_shared<yz_rect>(-300, 880, -300, 880, -200, red));
    objects.add(make_shared<Sphere>(glm::dvec3(0,10,10), 50, light));
    objects.add(make_shared<xz_rect>(-200, 880, -200, 880, -200, white));
    objects.add(make_shared<xz_rect>(-200, 880, -200, 880, 880, white));
    objects.add(make_shared<xy_rect>(-200, 880, -200, 880, 880, white));

    return objects;
}

int main(){
    
    std::cout << "Completed Project is running" << std::endl << std::endl;

    // Image
    // Rendered Images will be in 640×480 pixels
    PixelBuffer pixel_buffer(glm::ivec2(640, 480));

    
    // World
    hittable_list world;
    world.clear();
    
    //Materials
    auto material_ground = make_shared<lambertian>(glm::dvec3(0.5, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(glm::dvec3(0.9, 0.3, 0.3));
    auto material_left   = make_shared<metal>(glm::dvec3(0.8, 0.5, 0.5), 0.3);
    auto material_right  = make_shared<metal>(glm::dvec3(0.8, 0.6, 0.2),1.0);
    auto material_right2 =  make_shared<dielectric>(1.0);
    
    // Lights
    //auto difflight = make_shared<diffuse_light>(glm::dvec3(4,4,4));
    auto difflight2 = make_shared<diffuse_light>(glm::dvec3(4,0,0));
    auto reclight= make_shared<diffuse_light>(glm::dvec3(4,4,4));

    
    // World Objects
    world.add(make_shared<Sphere>(glm::dvec3(0,3,3), -1.1, material_right2));
    //world.add(make_shared<Sphere>(glm::dvec3(-1,0,-1), 2.0, material_center));
    world.add(make_shared<Sphere>(glm::dvec3(-1,3.5,0), 0.5, material_left));
    world.add(make_shared<Sphere>(glm::dvec3(-1,0,3), 1.0,material_right));
    world.add(make_shared<Sphere>(glm::dvec3(0,-100.5,-1), 100.0,material_ground));
    // Cube
    world.add(make_shared<box>(glm::dvec3(1, 0, 5), glm::dvec3(5,5, 8), material_center));
    
    
    // Adding lights
    //world.add(make_shared<Sphere>(glm::dvec3(0,3,3), -1.0, difflight));
    //world.add(make_shared<Sphere>(glm::dvec3(-3,3,-1), 0.5, difflight2));
    world.add(make_shared<Sphere>(glm::dvec3(-3,6,-1), 0.5, difflight2));
    //world.add(make_shared<xy_rect>(3, 5, 1, 8, -2, reclight));
    world.add(make_shared<box>(glm::dvec3(-1, 0, -3), glm::dvec3(1,2, 0), reclight));
    
    // Camera
    Camera camera(glm::dvec3(-10, 6, 2), glm::dvec3(25, -2, 0), pixel_buffer);
    
    glm::dvec3 background(0,0,0);

    //TASK 7
    // Cornell-box
    //hittable_list world2;
    //world2=cornell_box();
    //Camera camera2(glm::dvec3(278, 278, -800), glm::dvec3(278, 278, 0), pixel_buffer);
    
    
    int max_depth=100;
    for (int y = 0; y < pixel_buffer.dimensions.y; ++y)
    {
        for (int x = 0; x < pixel_buffer.dimensions.x; ++x)
        {
            glm::dvec3 color(0);
            glm::dvec3 pixel_color(0);
            
            double sample_per_pixel = 100.0;
            
            // Anti-Aliasing
            
           
            // Random algorithm selects points in the pixel to shoot rays through at random (Stochastic Sampling)
            for (int i = 0; i < sample_per_pixel; ++i) {
                
                glm::dvec2 uv(x + drand48(), y+ drand48());
                uv /= pixel_buffer.dimensions; // Normalization by elemet-wise divison
                
                // Starting from camera's position, the eye towards the pixel, the direction
                Ray ray(camera.position, glm::normalize(camera.raster_to_world(uv) - camera.position));
               
                // Ray-color
                pixel_color += ray_color(ray,background,world,max_depth);
                
                //Task 1 & 2 & 3 || Coloring according to its normals
                // pixel_color += colored_normals(ray, world);
            }
            
            
            /*
            // Grid Algorithm
            for (int i= 0; i < sqrt(sample_per_pixel);  ++i) {
                for (int j= 0; j < sqrt(sample_per_pixel);  ++j) {
                    
                    glm::dvec2 uv(x - 0.5 + (1/ (2 * sqrt(sample_per_pixel))) + (j/sqrt(sample_per_pixel)) , y - 0.5 + (1/( 2 * sqrt(sample_per_pixel)) )+ (i/sqrt(sample_per_pixel) ));
                    uv /= pixel_buffer.dimensions; // Normalization by elemet-wise divison
                    
                    // Starting from camera's position, the eye towards the pixel, the direction
                    Ray ray(camera.position, glm::normalize(camera.raster_to_world(uv) - camera.position));
                    
                    // Ray-color
                    pixel_color += ray_color(ray,world);
                    
                     //Task 1 & 2 & 3 || Coloring according to its normals
                     // pixel_color += colored_normals(ray, world);
                }
            }
             */

            pixel_color /= sample_per_pixel;
            color += sqrt(pixel_color); //gamma correction
            
            // Upside down the values of y axis, so it will be started from the left bottom corner
            pixel_buffer.set(x, pixel_buffer.dimensions.y - y - 1, color);
        }
    }
    
    // Render
    // Output Rendered Image in ppm format
    std::ofstream output;
    output.open("./render.ppm", std::ios::out | std::ios::trunc);
    if (!output.is_open())
        return 1;

    std::cout << "Outputting...";
    IO::write_as_PPM(pixel_buffer, output);
    output.close();
    std::cout << "done!" << std::endl;
    
    return 0;
}
