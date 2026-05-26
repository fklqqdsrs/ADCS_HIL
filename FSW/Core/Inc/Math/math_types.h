#pragma once

#include "Config/compiler_policy.h"
#include <Eigen/Core>
#include <cstdint>

namespace adcs::math
{
using Scalar = float;
using Vec3  = Eigen::Matrix<Scalar, 3, 1>;
using Vec4  = Eigen::Matrix<Scalar, 4, 1>;
using Mat3  = Eigen::Matrix<Scalar, 3, 3>;
using Mat4  = Eigen::Matrix<Scalar, 4, 4>;
using Mat43 = Eigen::Matrix<Scalar, 4, 3>;
using Mat34 = Eigen::Matrix<Scalar, 3, 4>;
using quat  = Eigen::Quaternion<Scalar>;

struct RawVec3 final
{
    float x;
    float y;
    float z;
};

struct RawQuat final
{
    float w;
    float x;
    float y;
    float z;
};

struct wheelVec final
{
    float wh1;
    float wh2;
    float wh3;
    float wh4;
};

inline Vec3 toEigen(const RawVec3& v) noexcept
{
    return Vec3{v.x, v.y, v.z};
}

inline RawVec3 toRaw(const Vec3& v) noexcept
{
    return RawVec3{v(0), v(1), v(2)};
}

inline Vec4 quatToEigen(const RawQuat& q) noexcept
{
    return Vec4{q.w, q.x, q.y, q.z};
}

inline RawQuat quatToRaw(const Vec4& q) noexcept
{
    return RawQuat{q(0), q(1), q(2), q(3)};
}
}
