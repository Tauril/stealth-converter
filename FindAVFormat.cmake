include(LibAVFindComponent)

libav_find_component("Format")

# Handle arguments
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(AVFormat
  DEFAULT_MSG
  AVFormat_LIBRARIES
  AVFormat_INCLUDE_DIRS
)

mark_as_advanced(AVFormat_LIBRARIES AVFormat_INCLUDE_DIRS)
