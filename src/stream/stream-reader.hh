#pragma once

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>
#include <utility>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfiltergraph.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
}

#include "stream-data.hh"

namespace stream
{

  namespace reader
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

        const std::vector<std::pair<int, int>>& iframes_split_get() const;

      private:
        StreamReader(const std::string& video);

        static constexpr const char* filter_ = "select='eq(pict_type,I)'";

        AVFormatContext* format_ctx_      = nullptr;
        AVCodecContext* codec_ctx_        = nullptr;
        AVFilterContext* buffer_sink_ctx_ = nullptr;
        AVFilterContext* buffer_src_ctx_  = nullptr;
        AVFilterGraph* filter_graph_      = nullptr;
        int vstream_idx_;

        std::vector<std::pair<int, int>> iframes_split_;
    };

  } // namespace reader

} // namespace stream

#include "stream-reader.hxx"
