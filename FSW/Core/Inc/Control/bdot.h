#pragma once

#include "Config/adcs_config.h"
#include "Math/math_types.h"
#include "App/messages.h"
#include <algorithm>
#include <cstdint>

namespace adcs::ctrl
{
class BdotController final
{
public:
    void init(const float gain_am2_per_tps = kDefaultGainAm2PerTps) noexcept;
    void reset() noexcept;
    app::MagCommand compute(const app::MagSample& sample) noexcept;

    static constexpr float kDefaultGainAm2PerTps = 250000.0F;

private:
    bool has_previous_sample_{false};
    std::uint64_t previous_time_ms_{0U};
    math::Vec3 previous_mag_tesla_{0.0F, 0.0F, 0.0F};
    float gain_am2_per_tps_{kDefaultGainAm2PerTps};
};

namespace detail
{
inline float saturateDipole(const float value) noexcept
{
    return std::max(-cfg::kMaxMagDipoleAm2, std::min(cfg::kMaxMagDipoleAm2, value));
}

inline math::Vec3 saturateDipoleVec3(const math::Vec3& value) noexcept
{
    return math::Vec3{saturateDipole(value(0)),
                      saturateDipole(value(1)),
                      saturateDipole(value(2))};
}

inline float deltaTimeSeconds(const std::uint64_t newer_time_ms,
                              const std::uint64_t older_time_ms) noexcept
{
    return static_cast<float>(newer_time_ms - older_time_ms) * 1.0E-3F;
}
}

inline void BdotController::init(const float gain_am2_per_tps) noexcept
{
    gain_am2_per_tps_ = gain_am2_per_tps;
    reset();
}

inline void BdotController::reset() noexcept
{
    has_previous_sample_ = false;
    previous_time_ms_ = 0U;
    previous_mag_tesla_ = math::Vec3{0.0F, 0.0F, 0.0F};
}

inline app::MagCommand BdotController::compute(const app::MagSample& sample) noexcept
{
    app::MagCommand command{};
    command.time_ms = sample.time_ms;
    command.dipole_body_am2 = {0.0F, 0.0F, 0.0F};
    command.valid = false;

    if (!sample.valid)
    {
        return command;
    }

    const math::Vec3 mag_tesla = math::toEigen(sample.mag_tesla);

    if ((!has_previous_sample_) || (sample.time_ms <= previous_time_ms_))
    {
        previous_time_ms_ = sample.time_ms;
        previous_mag_tesla_ = mag_tesla;
        has_previous_sample_ = true;
        return command;
    }

    const float dt_sec = detail::deltaTimeSeconds(sample.time_ms, previous_time_ms_);
    const math::Vec3 b_dot_tps = (mag_tesla - previous_mag_tesla_) / dt_sec;
    const math::Vec3 dipole_body = detail::saturateDipoleVec3(-gain_am2_per_tps_ * b_dot_tps);

    previous_time_ms_ = sample.time_ms;
    previous_mag_tesla_ = mag_tesla;

    command.dipole_body_am2 = math::toRaw(dipole_body);
    command.valid = true;
    return command;
}
}
