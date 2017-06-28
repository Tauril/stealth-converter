#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace stream_reader
{

  class StreamReader
  {
    public:
      StreamReader(const std::string& video);

      const cv::VideoCapture& video_get() const;
      cv::VideoCapture& video_get();

    private:
      cv::VideoCapture video_;
  };

} // namespace stream_reader

#include "stream-reader.hxx"
