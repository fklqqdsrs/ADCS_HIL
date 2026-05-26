#pragma once

#include "Config/adcs_config.h"
#include "Math/math_types.h"
#include "App/messages.h"
#include <Eigen/Geometry>
#include <algorithm>

namespace adcs::ctrl
{
class DetumblingController final
{
public:
    void init(const float gain_am2_tesla_sec = kDefaultGainAm2TeslaSec) noexcept;
    app::MagCommand compute(const app::AdcsSnapshot& state,
                            const app::MagSample& mag_sample) const noexcept;

    static constexpr float kDefaultGainAm2TeslaSec = 0.001F;

private:
    float gain_am2_tesla_sec_{kDefaultGainAm2TeslaSec};
};

namespace detail
{
constexpr float kMinDetumblingMagFieldNormTesla = 1.0E-9F;

inline float saturateDetumblingDipole(const float value) noexcept
{
    return std::max(-cfg::kMaxMagDipoleAm2, std::min(cfg::kMaxMagDipoleAm2, value));
}

inline math::Vec3 saturateDetumblingDipoleVec3(const math::Vec3& value) noexcept
{
    return math::Vec3{saturateDetumblingDipole(value(0)),
                      saturateDetumblingDipole(value(1)),
                      saturateDetumblingDipole(value(2))};
}
}

inline void DetumblingController::init(const float gain_am2_tesla_sec) noexcept
{
    gain_am2_tesla_sec_ = std::max(0.0F, gain_am2_tesla_sec);
}

inline app::MagCommand DetumblingController::compute(const app::AdcsSnapshot& state,
                                                     const app::MagSample& mag_sample) const noexcept
{
    app::MagCommand command{};
    command.time_ms = mag_sample.time_ms;
    command.dipole_body_am2 = {0.0F, 0.0F, 0.0F};
    command.valid = false;

    if ((!state.attitude_valid) || (!mag_sample.valid) || (gain_am2_tesla_sec_ <= 0.0F))
    {
        return command;
    }

    const math::Vec3 mag_body_tesla = math::toEigen(mag_sample.mag_tesla);
    const float mag_norm_tesla = mag_body_tesla.norm();
    if (mag_norm_tesla <= detail::kMinDetumblingMagFieldNormTesla)
    {
        return command;
    }

    const math::Vec3 b_unit = mag_body_tesla / mag_norm_tesla;
    const math::Vec3 omega_body_radps = math::toEigen(state.omega_body_radps);
    const math::Vec3 dipole_body = detail::saturateDetumblingDipoleVec3(
        (gain_am2_tesla_sec_ / mag_norm_tesla) * omega_body_radps.cross(b_unit));

    command.dipole_body_am2 = math::toRaw(dipole_body);
    command.valid = true;
    return command;
}
}
