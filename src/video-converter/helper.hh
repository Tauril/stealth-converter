#pragma once

#include <type_traits>

namespace constants
{
    const double m_pi = 3.14159265358979323846;
    const double m_pi2 = 1.57079632679489661923;
    const double m_pi4 = 0.78539816339744830962;

    // sphere radius
    const int sphere_radius = 1.0;
}

namespace help
{
    /** \brief Generic function to return the int underlying an enum class
     * (basically a static cast)
     *
     * \param Enumeration const value Value of an enum class
     * \return int
     */
    template <typename Enumeration>
    auto as_integer(Enumeration const value) -> typename std::underlying_type<Enumeration>::type;

    /** \brief Test if double value in in the range of [small, large]
     *
     * \param value const double& the value to test
     * \param small const double& the small boundary
     * \param large const double& the large boundary
     * \param epsilon const double& the error margin
     * \return bool true if in range
     */
    inline bool isDoubleInRange(const double& value, const double& small,
                                    const double& large, const double& epsilon);

}

#include "helper.hxx"