#pragma once

#include "stream-reader.hh"

namespace stream_reader
{

  inline
  const cv::VideoCapture&
  StreamReader::video_get() const
  {
    return video_;
  }

  inline
  cv::VideoCapture&
  StreamReader::video_get()
  {
    return video_;
  }

  inline
  const std::string&
  StreamReader::video_name_get() const
  {
    return video_name_;
  }

} // namespace stream_reader
