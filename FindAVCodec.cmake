include(LibAVFindComponent)

libav_find_component("Codec")

# Handle arguments
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(AVCodec
  DEFAULT_MSG
  AVCodec_LIBRARIES
  AVCodec_INCLUDE_DIRS
)

mark_as_advanced(AVCodec_LIBRARIES AVCodec_INCLUDE_DIRS)
