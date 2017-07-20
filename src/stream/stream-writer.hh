#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "stream-data.hh"
#include "stream-reader.hh"

#include <tbb/parallel_for_each.h>

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

      private:
        StreamWriter() = default;
    };

  } // namespace writer

} // namespace stream
