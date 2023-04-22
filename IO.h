//
//  IO.h
//  ex4
//
//  Created by Begum Celik on 31.10.2020.
//

#ifndef IO_h
#define IO_h


#endif /* IO_h */

#pragma once

#include <iostream>
#include <fstream>

#include "PixelBuffer.h"

namespace IO
{
    void write_as_PPM(const PixelBuffer& pixel_buffer, std::ostream& output)
    {
        output << "P3\n" << pixel_buffer.dimensions.x << ' ' << pixel_buffer.dimensions.y << "\n255\n";

        const int total_pixels = pixel_buffer.dimensions.x * pixel_buffer.dimensions.y;
        for (int i = 0; i < total_pixels; ++i)
        {
            auto v = pixel_buffer.get(i) * 255.99;

            int ir = static_cast<int>(v.r);
            int ig = static_cast<int>(v.g);
            int ib = static_cast<int>(v.b);

            output << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
}
