#pragma once

#include "stream-data.hh"

namespace stream
{

  inline
  const std::string&
  StreamData::video_name_get() const
  {
    return video_name_;
  }

  inline
  void
  StreamData::fps_set(int fps)
  {
    fps_ = fps;
  }

  inline
  int
  StreamData::fps_get() const
  {
    return fps_;
  }

  inline
  void
  StreamData::nb_frames_set(int nb_frames)
  {
    nb_frames_ = nb_frames;
  }

  inline
  int
  StreamData::nb_frames_get() const
  {
    return nb_frames_;
  }

  inline
  void
  StreamData::width_set(int width)
  {
    width_ = width;
  }

  inline
  int
  StreamData::width_get() const
  {
    return width_;
  }

  inline
  void
  StreamData::height_set(int height)
  {
    height_ = height;
  }

  inline
  int
  StreamData::height_get() const
  {
    return height_;
  }

} // namespace stream
