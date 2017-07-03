include(LibAVFindComponent)

libav_find_component("Filter")

find_library(SWSCALE_LIBRARY
  NAMES swscale libswscale
  HINTS ${AVFilter_LIBRARY_DIRS}
)
list(APPEND AVFilter_LIBRARIES ${SWSCALE_LIBRARY})

# Handle arguments
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(AVFilter
  DEFAULT_MSG
  AVFilter_LIBRARIES
  AVFilter_INCLUDE_DIRS
)

mark_as_advanced(AVFilter_LIBRARIES AVFilter_INCLUDE_DIRS)
