#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <tbb/parallel_for_each.h>

#include <vector>

#include "stream-data.hh"
#include "stream-reader.hh"

namespace stream
{

  namespace writer
  {

    class StreamWriter
    {
      public:
        static StreamWriter& Instance();

        StreamWriter(const StreamWriter&) = delete;
        StreamWriter& operator=(const StreamWriter&) = delete;

        void split_video() const;
        void construct_video(
                      const std::vector<std::string>& sub_videos) const;

      private:
        StreamWriter() = default;

        const std::string video_prefix_ = "__stealth_reader_output_";
        const std::string video_suffix_ = ".mp4";
    };

  } // namespace writer

} // namespace stream
