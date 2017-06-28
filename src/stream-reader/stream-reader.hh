#pragma once

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace stream_reader
{

  class StreamReader
  {
    public:
      StreamReader(const std::string& video);

      void run();

      const cv::VideoCapture& video_get() const;
      cv::VideoCapture& video_get();

      const std::string& video_name_get() const;

    private:
      cv::VideoCapture video_;
      std::string video_name_;
  };

} // namespace stream_reader

#include "stream-reader.hxx"
