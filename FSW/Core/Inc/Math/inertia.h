#pragma once

#include "Config/adcs_config.h"
#include "Math/math_types.h"
#include <Eigen/Geometry>

namespace adcs::math
{
inline Mat3 makeInertiaBody() noexcept
{
    Mat3 inertia{};
    inertia << cfg::kIxx, cfg::kIxy, cfg::kIxz,
               cfg::kIxy, cfg::kIyy, cfg::kIyz,
               cfg::kIxz, cfg::kIyz, cfg::kIzz;
    return inertia;
}

inline Vec3 gyroscopicTorque(const Mat3& inertia_body, const Vec3& omega_body_radps) noexcept
{
    return omega_body_radps.cross(inertia_body * omega_body_radps);
}
}
