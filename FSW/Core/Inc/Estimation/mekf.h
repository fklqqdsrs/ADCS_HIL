#pragma once

#include "App/messages.h"
#include "Math/math_types.h"
#include "Math/quaternion.h"
#include <cstdint>

namespace adcs::est
{
class Mekf final
{
public:
    void reset(const app::AdcsSnapshot& initial_state) noexcept;
    void propagate(const app::ImuSample& imu, float dt_sec) noexcept;
    void updateStarTracker(const app::StarTrackerSample& star_tracker) noexcept;
    void updateGnss(const app::GnssSample& gnss) noexcept;
    app::AdcsSnapshot snapshot() const noexcept;

private:
    math::Vec4 q_body_to_eci_{1.0F, 0.0F, 0.0F, 0.0F};
    math::Vec3 omega_body_radps_{0.0F, 0.0F, 0.0F};
    math::Vec3 r_eci_m_{0.0F, 0.0F, 0.0F};
    math::Vec3 v_eci_mps_{0.0F, 0.0F, 0.0F};
    bool attitude_valid_{false};
    bool orbit_valid_{false};
    std::uint64_t time_ms_{0U};
};

inline void Mekf::reset(const app::AdcsSnapshot& initial_state) noexcept
{
    q_body_to_eci_ = math::quatNormalize(math::quatToEigen(initial_state.q_body_to_eci));
    omega_body_radps_ = math::toEigen(initial_state.omega_body_radps);
    r_eci_m_ = math::toEigen(initial_state.r_eci_m);
    v_eci_mps_ = math::toEigen(initial_state.v_eci_mps);
    attitude_valid_ = initial_state.attitude_valid;
    orbit_valid_ = initial_state.orbit_valid;
    time_ms_ = initial_state.time_ms;
}

inline void Mekf::propagate(const app::ImuSample& imu, const float dt_sec) noexcept
{
    if (!imu.valid)
    {
        return;
    }

    const math::Vec3 omega = math::toEigen(imu.gyro_radps);
    const float half_dt = 0.5F * dt_sec;

    const math::Vec4 dq{1.0F,
                        half_dt * omega(0),
                        half_dt * omega(1),
                        half_dt * omega(2)};

    q_body_to_eci_ = math::quatNormalize(math::quatMultiply(q_body_to_eci_, dq));
    omega_body_radps_ = omega;
    time_ms_ = imu.time_ms;
    attitude_valid_ = true;
}

inline void Mekf::updateStarTracker(const app::StarTrackerSample& star_tracker) noexcept
{
    if (star_tracker.valid)
    {
        q_body_to_eci_ = math::quatNormalize(math::quatToEigen(star_tracker.q_body_to_eci));
        omega_body_radps_ = math::toEigen(star_tracker.omega_body_radps);
        time_ms_ = star_tracker.time_ms;
        attitude_valid_ = true;
    }
}

inline void Mekf::updateGnss(const app::GnssSample& gnss) noexcept
{
    if (gnss.valid)
    {
        r_eci_m_ = math::toEigen(gnss.r_eci_m);
        v_eci_mps_ = math::toEigen(gnss.v_eci_mps);
        time_ms_ = gnss.time_ms;
        orbit_valid_ = true;
    }
}

inline app::AdcsSnapshot Mekf::snapshot() const noexcept
{
    return app::AdcsSnapshot{time_ms_,
                             app::AdcsMode::Nominal,
                             math::quatToRaw(q_body_to_eci_),
                             math::toRaw(omega_body_radps_),
                             math::toRaw(r_eci_m_),
                             math::toRaw(v_eci_mps_),
                             attitude_valid_,
                             orbit_valid_};
}
}
