#pragma once

#include <string>

namespace stream
{

  class StreamData
  {
    public:
      static StreamData& Instance(const std::string& video_name = "");

      StreamData& operator=(const StreamData&) = delete;
      StreamData(const StreamData&) = delete;

      const std::string& video_name_get() const;
      int fps_get() const;
      int nb_frames_get() const;
      int width_get() const;
      int height_get() const;

      void fps_set(int fps);
      void nb_frames_set(int nb_frames);
      void width_set(int width);
      void height_set(int height);

    private:
      StreamData(const std::string& video);

      std::string video_name_;
      int fps_;
      int nb_frames_;
      int width_;
      int height_;
  };

} // namespace stream

#include "stream-data.hxx"
