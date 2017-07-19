#pragma once

#include <iostream>
#include <cmath>

namespace convert
{
    /** This class represents the main processing step to convert a 2D video into a "false" 3D video
     ** The main method of this class is .....
     ** The input of this method is a 2D frame and a noise sample. Default noise sanmple is transparent.
     ** The output is an equirectangle frame with a 2:1 aspect ratio and 7168:3584 or higher resolution
     ** (YouTube format)
     **
     ** The aim is to use the input frames and noises as a cubemap and convert it to equirectangle.
     ** The next step ought to be grouping all the equirectangle frames into a video with ffmpeg
     */
    class Converter
    {
        /** General Procedure:
         ** for each pixel of the destination image:
         **     calculate the corresponding unit vector in 3-dimensional space
         **     calculate the x,y coordinate for that vector in the source image
         **     sample the source image at that coordinate and assign the value to the destination pixel
         */
        public:
            Converter() = default;
         /** 2 Options : Transparent all pixels value at 0 (0, 0, 0, 0)
          ** Noise generated :  rand() to add noise, and conv2() or imfilter() to blur the image.
          */

          // + option parallel or not with TBB

            // compute(picture + Transparent or noise) returns Equirectangle
    }
}