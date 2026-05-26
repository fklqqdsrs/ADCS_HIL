#pragma once

#include "Config/adcs_config.h"
#include "Math/inertia.h"
#include "App/messages.h"
#include "Math/math_types.h"
#include "Math/quaternion.h"
#include <algorithm>

namespace adcs::ctrl
{
class QuatPdController final
{
public:
    void init() noexcept;
    app::BodyTorqueCommand compute(const app::AdcsSnapshot& state,
                                   const app::TargetAttitude& target) const noexcept;

private:
    math::Mat3 inertia_body_{};
    math::Vec3 kp_{0.0F, 0.0F, 0.0F};
    math::Vec3 kd_{0.0F, 0.0F, 0.0F};
};

namespace detail
{
inline float saturate(const float value, const float limit) noexcept
{
    return std::max(-limit, std::min(limit, value));
}

inline math::Vec3 saturateVec3(const math::Vec3& value, const float limit) noexcept
{
    return math::Vec3{saturate(value(0), limit),
                      saturate(value(1), limit),
                      saturate(value(2), limit)};
}
}

inline void QuatPdController::init() noexcept
{
    inertia_body_ = math::makeInertiaBody();
    kp_ = math::Vec3{cfg::kKpX, cfg::kKpY, cfg::kKpZ};
    kd_ = math::Vec3{cfg::kKdX, cfg::kKdY, cfg::kKdZ};
}

inline app::BodyTorqueCommand QuatPdController::compute(const app::AdcsSnapshot& state,
                                                        const app::TargetAttitude& target) const noexcept
{
    app::BodyTorqueCommand command{};
    command.time_ms = state.time_ms;
    command.valid = false;
    command.torque_body_nm = {0.0F, 0.0F, 0.0F};

    if ((!state.attitude_valid) || (!target.valid))
    {
        return command;
    }

    const math::Vec4 q_body_to_eci = math::quatToEigen(state.q_body_to_eci);
    const math::Vec4 q_target_to_eci = math::quatToEigen(target.q_target_to_eci);
    const math::Vec4 q_err = math::quatErrorTargetToBody(q_body_to_eci, q_target_to_eci);
    const math::Vec3 e = math::quatVectorPartShortest(q_err);

    const math::Vec3 omega_body = math::toEigen(state.omega_body_radps);
    const math::Vec3 omega_target = math::toEigen(target.omega_target_body_radps);
    const math::Vec3 omega_error = omega_body - omega_target;

    const math::Vec3 torque_pd = (-2.0F * kp_.cwiseProduct(e)) - kd_.cwiseProduct(omega_error);

    const math::Vec3 gyro_ff = math::gyroscopicTorque(inertia_body_, omega_body);
    const math::Vec3 alpha_target = math::toEigen(target.omega_dot_target_body_radps2);
    const math::Vec3 alpha_ff = inertia_body_ * alpha_target;

    const math::Vec3 torque = detail::saturateVec3(torque_pd + gyro_ff + alpha_ff, cfg::kMaxBodyTorqueNm);
    command.torque_body_nm = math::toRaw(torque);
    command.valid = true;
    return command;
}
}
