#pragma once

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfiltergraph.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
}

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace stream_reader
{

  class StreamReader
  {
    public:
      static StreamReader& Instance(const std::string& video = "");

      StreamReader(const StreamReader&) = delete;
      StreamReader& operator=(const StreamReader&) = delete;

      ~StreamReader();

      void open_input_file();
      void init_filter_graph();

      void get_iframes();

      const std::string& video_get() const;

    private:
      StreamReader(const std::string& video);

      std::string video_;

      static constexpr const char* filter_ = "select='eq(pict_type,I)'";

      AVFormatContext* format_ctx_ = nullptr;
      AVCodecContext* codec_ctx_ = nullptr;
      AVFilterContext* buffer_sink_ctx_ = nullptr;
      AVFilterContext* buffer_src_ctx_ = nullptr;
      AVFilterGraph* filter_graph_ = nullptr;
      int vstream_idx_;
  };

} // namespace stream_reader

#include "stream-reader.hxx"
