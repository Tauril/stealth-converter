#include "stream-data.hh"

namespace stream
{

  StreamData&
  StreamData::Instance(const std::string& video_name)
  {
    static StreamData instance(video_name);
    return instance;
  }

  StreamData::StreamData(const std::string& video_name)
    : video_name_(video_name)
  {};

} // namespace stream
