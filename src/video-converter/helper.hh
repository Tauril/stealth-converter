#pragma once

#include <type_traits>

namespace help
{
    template <typename Enumeration>
    auto as_integer(Enumeration const value) -> typename std::underlying_type<Enumeration>::type;

    /** Test if double value in in the range of [small, large)
     */
    inline bool isDoubleInRange(const double& value, const double& small,
                                    const double& large, const double& epsilon);

}

#include "helper.hxx"