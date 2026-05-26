#pragma once

#include "Config/adcs_config.h"
#include "Math/constant.h"
#include "App/messages.h"
#include "Math/math_types.h"
#include <algorithm>
#include <cstdint>

namespace adcs::act
{
class WheelAllocator final
{
public:
    void init() noexcept;
    app::WheelTorqueCommand allocate(const app::BodyTorqueCommand& body_command) const noexcept;
    app::MagCommand zeroMagCommand(std::uint64_t time_ms) const noexcept;

private:
    math::Mat34 wheel_axes_body_{}; 
};

namespace detail
{
inline float saturate(const float value, const float limit) noexcept
{
    return std::max(-limit, std::min(limit, value));
}

inline app::WheelSpeedCommand TorquecommandToSpeed(const app::WheelTorqueCommand& wheel_command,math::wheelVec wheel_previous_speed, float dt) noexcept  
{
    app::WheelSpeedCommand command{};

    command.wheel_speed_mrpm[0] = static_cast<std::int32_t>(wheel_previous_speed.wh1 + (wheel_command.wheel_torque_nm[0] / cfg::kWheelInertia) * dt * math::constant::radianPerSecToMrpm);
    command.wheel_speed_mrpm[1] = static_cast<std::int32_t>(wheel_previous_speed.wh2 + (wheel_command.wheel_torque_nm[1] / cfg::kWheelInertia) * dt * math::constant::radianPerSecToMrpm);
    command.wheel_speed_mrpm[2] = static_cast<std::int32_t>(wheel_previous_speed.wh3 + (wheel_command.wheel_torque_nm[2] / cfg::kWheelInertia) * dt * math::constant::radianPerSecToMrpm);
    command.wheel_speed_mrpm[3] = static_cast<std::int32_t>(wheel_previous_speed.wh4 + (wheel_command.wheel_torque_nm[3] / cfg::kWheelInertia) * dt * math::constant::radianPerSecToMrpm);
    return command;
}
}

inline void WheelAllocator::init() noexcept
{
    wheel_axes_body_ = cfg::makeWheelAxesBodyMatrix();

}

inline app::WheelTorqueCommand WheelAllocator::allocate(const app::BodyTorqueCommand& body_command) const noexcept
{
    app::WheelTorqueCommand wheel_command{};
    wheel_command.time_ms = body_command.time_ms;
    wheel_command.valid = false;
    wheel_command.wheel_torque_nm[0] = 0.0F;
    wheel_command.wheel_torque_nm[1] = 0.0F;
    wheel_command.wheel_torque_nm[2] = 0.0F;
    wheel_command.wheel_torque_nm[3] = 0.0F;

    if (!body_command.valid)
    {
        return wheel_command;
    }

    const math::Vec3 torque_body  =  math::toEigen(body_command.torque_body_nm);
    const math::Vec4 wheel_torque =  -cfg::makeBodyAxesWheelMatrix() * torque_body;

    wheel_command.wheel_torque_nm[0] = detail::saturate(wheel_torque(0), cfg::kMaxWheelTorqueNm);
    wheel_command.wheel_torque_nm[1] = detail::saturate(wheel_torque(1), cfg::kMaxWheelTorqueNm);
    wheel_command.wheel_torque_nm[2] = detail::saturate(wheel_torque(2), cfg::kMaxWheelTorqueNm);
    wheel_command.wheel_torque_nm[3] = detail::saturate(wheel_torque(3), cfg::kMaxWheelTorqueNm);
    wheel_command.valid = true;
    return wheel_command;

}

inline app::MagCommand WheelAllocator::zeroMagCommand(const std::uint64_t time_ms) const noexcept
{
    return app::MagCommand{time_ms, {0.0F, 0.0F, 0.0F}, true};
}
}
