#pragma once

#include "stream-reader.hh"

namespace stream_reader
{

  inline
  const std::string&
  StreamReader::video_get() const
  {
    return video_;
  }

} // namespace stream_reader
