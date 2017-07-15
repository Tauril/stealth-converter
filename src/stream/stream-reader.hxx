#pragma once

#include "stream-reader.hh"

namespace stream
{

  namespace reader
  {

    inline
    const std::vector<std::pair<int, int>>&
    StreamReader::iframes_split_get() const
    {
      return iframes_split_;
    }

  } // namespace reader

} // namespace stream
