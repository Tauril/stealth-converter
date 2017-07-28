#pragma once

#include <string>
#include "cube-coordinate.hh"
#include "convert.hh"

#include <tbb/tbb.h>

namespace convert
{
    /** This class represents the main processing step to convert a 2D video
     ** into a "false" 3D video
     ** The main method of this class is process
     ** The input of this method is a 2D frame and a noise sample. Default
     ** noise sanmple is transparent. (Maybe false #tbd)
     ** The output is an equirectangle frame with a 2:1 aspect ratio
     ** and 7168:3584 or higher resolution (YouTube format)
     **
     ** The aim is to use the input frames and noises as a cubemap and convert
     ** it to equirectangle.
     ** The next step ought to be grouping all the equirectangle frame
     ** into a video with ffmpeg
     */

    /** This is the Wrapper on convert that will do the following:
     ** 2 Options : Transparent all pixels value at 0 (0, 0, 0, 0)
     ** Noise generated :  rand() to add noise, and conv2() or
     ** imfilter() to blur the image.
     ** + option parallel or not with TBB
     ** compute(picture + Transparent or noise) returns Equirectangle
     */
    class FrameHandler
    {
    public:
        void process_video_file(const std::string& input_video);
        cv::Mat* process(std::vector<std::string> inputs);
        cv::Mat* process(std::vector<cv::Mat> inputs);
        cv::Mat* process(cv::Mat input);
    private:
        // this function is a hack
        inline cv::Size get_output_size(const cv::Mat&);
        inline void squarify(cv::Mat& img);
    };
}

#include "frame-handler.hxx"