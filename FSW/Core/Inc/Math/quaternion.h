#pragma once

#include "Math/math_types.h"
#include <cmath>

namespace adcs::math
{
inline Vec4 quatNormalize(const Vec4& q) noexcept
{
    const Scalar n2 = q.squaredNorm();
    if (n2 <= 1.0E-12F)
    {
        return Vec4{1.0F, 0.0F, 0.0F, 0.0F};
    }
    return q / std::sqrt(n2);
}

inline Vec4 quatConjugate(const Vec4& q) noexcept
{
    return Vec4{q(0), -q(1), -q(2), -q(3)};
}

inline Vec4 quatMultiply(const Vec4& a, const Vec4& b) noexcept
{
    const Scalar aw = a(0);
    const Scalar ax = a(1);
    const Scalar ay = a(2);
    const Scalar az = a(3);
    const Scalar bw = b(0);
    const Scalar bx = b(1);
    const Scalar by = b(2);
    const Scalar bz = b(3);

    return Vec4{
        (aw * bw) - (ax * bx) - (ay * by) - (az * bz),
        (aw * bx) + (ax * bw) + (ay * bz) - (az * by),
        (aw * by) - (ax * bz) + (ay * bw) + (az * bx),
        (aw * bz) + (ax * by) - (ay * bx) + (az * bw)};
}

inline Vec4 quatErrorTargetToBody(const Vec4& q_body_to_inertial,
                                  const Vec4& q_target_to_inertial) noexcept
{
    const Vec4 q_bi = quatNormalize(q_body_to_inertial);
    const Vec4 q_ti = quatNormalize(q_target_to_inertial);
    return quatNormalize(quatMultiply(quatConjugate(q_bi), q_ti));
}

inline Vec3 quatVectorPartShortest(const Vec4& q_error) noexcept
{
    Vec4 q = quatNormalize(q_error);
    if (q(0) < 0.0F)
    {
        q = -q;
    }
    return Vec3{q(1), q(2), q(3)};
}

inline Mat3 skew(const Vec3& v) noexcept
{
    Mat3 s{};
    s << 0.0F, -v(2), v(1),
         v(2), 0.0F, -v(0),
        -v(1), v(0), 0.0F;
    return s;
}
}
