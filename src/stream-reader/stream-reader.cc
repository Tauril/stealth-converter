#include "stream-reader.hh"

namespace stream_reader
{

  StreamReader::StreamReader(const std::string& video)
    : video_name_(video)
  {
    // Initialize FFmpeg library.
    av_register_all();

    auto ret = avformat_open_input(&in_ctx_, video.c_str(), nullptr, nullptr);
    ret = avformat_find_stream_info(in_ctx_, nullptr);

    AVCodec* vcodec = nullptr;
    ret = av_find_best_stream(in_ctx_, AVMEDIA_TYPE_VIDEO, -1, -1, &vcodec, 0);
    vstream_index_ = ret;

    vstream_ = in_ctx_->streams[vstream_index_];

    // Open video decoder context.
    ret = avcodec_open2(vstream_->codec, vcodec, nullptr);

    std::cout << "video:  " << video << std::endl
              << "format: " << in_ctx_->iformat->name << std::endl
              << "vcodec: " << vcodec->name << std::endl
              << "size:   " << vstream_->codec->width << 'x'
                            << vstream_->codec->height << std::endl
              << "fps:    " << av_q2d(vstream_->codec->framerate) << std::endl
              << "length: " << av_rescale_q(vstream_->duration,
                                            vstream_->time_base,
                                            {1, 1000}) / 1000
                            << " sec" << std::endl
              << "frame:  " << vstream_->nb_frames << std::endl;

    width_ = vstream_->codec->width;
    height_ = vstream_->codec->height;
    const AVPixelFormat pix_fmt = AV_PIX_FMT_BGR24;

    sws_ctx_ = sws_getCachedContext(
      nullptr, width_, height_, vstream_->codec->pix_fmt, width_, height_,
      pix_fmt, SWS_BICUBIC, nullptr, nullptr, nullptr);

    std::cout << "output: " << width_ << 'x' << height_ << ", "
              << av_get_pix_fmt_name(pix_fmt) << std::endl;

    // Allocate frame buffer for output.
    frame_ = av_frame_alloc();
    frame_buff_.resize(avpicture_get_size(pix_fmt, width_, height_));
    avpicture_fill(reinterpret_cast<AVPicture*>(frame_), frame_buff_.data(),
                   pix_fmt, width_, height_);

    dec_frame_ = av_frame_alloc();
  };

  StreamReader::~StreamReader()
  {
    av_frame_free(&dec_frame_);
    av_frame_free(&frame_);
    avcodec_close(vstream_->codec);
    avformat_close_input(&in_ctx_);
    sws_freeContext(sws_ctx_);
  };

  void
  StreamReader::read()
  {
    unsigned nb_frames = 0;
    bool end_of_stream = false;
    int got_pic = 0;
    int ret = 0;

    auto pkt_del =
      [](AVPacket* packet) { av_free_packet(packet); delete packet; };
    do {
      std::unique_ptr<AVPacket, decltype(pkt_del)> pkt(new AVPacket, pkt_del);

      if (!end_of_stream)
      {
        ret = av_read_frame(in_ctx_, pkt.get());
        if (ret == 0 && pkt->stream_index != vstream_index_)
          continue;
        end_of_stream = (ret == AVERROR_EOF);
      }

      if (end_of_stream)
      {
        av_init_packet(pkt.get());
        pkt->data = nullptr;
        pkt->size = 0;
      }

      avcodec_decode_video2(vstream_->codec, dec_frame_, &got_pic, pkt.get());
      if (!got_pic)
        continue;

      sws_scale(sws_ctx_, dec_frame_->data, dec_frame_->linesize, 0,
                dec_frame_->height, frame_->data, frame_->linesize);

      /*
      // OpenCV to display the frame.
      cv::Mat frame(height_, width_, CV_8UC3, frame_buff_.data(),
                    frame_->linesize[0]);
      cv::imshow(video_name_, frame);

      if (cv::waitKey(30) >= 1000)
        break;*/

      std::cout << nb_frames++ << std::endl;
    } while (!end_of_stream || got_pic);
    std::cout << nb_frames << " frames decoded" << std::endl;
  }

  void
  StreamReader::filter_iframes()
  {
    AVFilter* buffer = avfilter_get_by_name("buffer");
    AVFilter* buffer_sink = avfilter_get_by_name("buffer_sink");

    AVFilter* select  = avfilter_get_by_name("select");
  }

} // namespace stream_reader
