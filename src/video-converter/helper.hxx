#include <cmath>

namespace help
{
    template <typename Enumeration>
    auto as_integer(Enumeration const value)
    -> typename std::underlying_type<Enumeration>::type
    {
        return static_cast<typename std::underlying_type<Enumeration>::type>(value);
    }

    inline bool
    isDoubleInRange(const double& value, const double& small,
                               const double& large, const double& epsilon) {
        bool equalSup = ((value - small) > 0) || (std::fabs(value - small) < epsilon);
        bool smaller = ((value - large) < 0) && !(std::fabs(value - large) < epsilon);
        return equalSup && smaller;
    }
}