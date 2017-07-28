#pragma once

#include <string>
#include "cube-coordinate.hh"
#include "convert.hh"

#include <tbb/tbb.h>

namespace convert
{
    /** This class represents the main processing step to convert a 2D video
     ** into a "false" 3D video
     ** The main method of this class is process_video_file
     ** The output is an equirectangle video with a 2:1 aspect ratio
     ** and ideally 7168:3584 or higher resolution (YouTube format)
     **
     ** The aim is to use the input frames as a cubemap and convert
     ** it to equirectangle.
     ** The equirectangle frames are reassembled into a video.
     */
    class FrameHandler
    {
    public:
        /** \brief Embed a video into a 360 video.
         *
         * \param input_video const std::string& video file
         * \return void
         */
        void process_video_file(const std::string& input_video);
        /** \brief process either one frame or multiple frames as a cubemap.
         *
         * \param inputs std::vector<std::string&> list of image file names
         * \return cv::Mat* image processed as an equirectangle
         */
        cv::Mat* process(std::vector<std::string> inputs);
        /** \brief process either one frame or multiple frames as a cubemap.
         *
         * \param inputs std::vector<cv::Mat&> list of image file
         * \return cv::Mat* image processed as an equirectangle
         */
        cv::Mat* process(std::vector<cv::Mat> inputs);
        /** \brief process one frame to a frame embedded in an equirectangle picture.
         *
         * \param input cv::Mat& image file
         * \return cv::Mat* image processed as an equirectangle
         */
        cv::Mat* process(cv::Mat input);
    private:
        /** \brief get the final frame dimensions (post processing) 
         *  to instanciate the videoWriterbefore conversion. rdInv and rdInH
         *  ought to be the same as the values used in the Converter instanciation.
         *
         * \param input cv::Mat& image file
          * \param rdInV const          double   The radian of the view portion
         * vertically, range [0.01, PI]
         * \param rdInH const          double   The radian of the view portion
         * horizontally, range [0.01, 2*PI]
         * \return cv::Size the size
         */
        inline cv::Size get_output_size(const cv::Mat&, double rdInV, double rdInH);
        /** \brief return a square picture if it isn't. Add black pixels padding
         *
         * \param input cv::Mat& image file
         * \return void
         */
        inline void squarify(cv::Mat& img);
    };
}

#include "frame-handler.hxx"