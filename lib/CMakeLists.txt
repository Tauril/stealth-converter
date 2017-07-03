find_package(AVFormat)
find_package(AVFilter)
find_package(AVCodec)

include_directories(
  ${AVFORMAT_INCLUDE_DIR}
  ${AVFILTER_INCLUDE_DIR}
  ${AVCODEC_INCLUDE_DIR}
)

set(
  FFMPEG_LIBS
  ${AVFORMAT_LIBRARY}
  ${AVFILTER_LIBRARY}
  ${AVCODEC_LIBRARY}
)
