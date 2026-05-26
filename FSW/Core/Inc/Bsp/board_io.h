#pragma once

#include "App/messages.h"
#include <cstdint>
#include "iwdg.h"
#include "main.h"

namespace adcs::bsp
{
namespace detail
{
extern std::uint64_t g_fake_time_ms;
}

inline void boardInit() noexcept
{
}

inline std::uint64_t timeNowMs() noexcept
{
    detail::g_fake_time_ms += 1U;
    return detail::g_fake_time_ms;
}

inline void feedWatchdog() noexcept
{
    HAL_IWDG_Refresh(&hiwdg1);
    //HAL_GPIO_TogglePin();
}

inline bool readImu(app::ImuSample& sample) noexcept
{
    sample.time_ms = timeNowMs();
    sample.gyro_radps = {0.0F, 0.0F, 0.0F};
    sample.accel_mps2 = {0.0F, 0.0F, 0.0F};
    sample.valid = true;
    return true;
}

inline bool readMag(app::MagSample& sample) noexcept
{
    sample.time_ms = timeNowMs();
    sample.mag_tesla = {0.0F, 0.0F, 0.0F};
    sample.valid = true;
    return true;
}

inline bool readStarTracker(app::StarTrackerSample& sample) noexcept
{
    sample.time_ms = timeNowMs();
    sample.q_body_to_eci = {1.0F, 0.0F, 0.0F, 0.0F};
    sample.omega_body_radps = {0.0F, 0.0F, 0.0F};
    sample.attitude_sigma_rad = 3.2E-5F;
    sample.valid = true;
    return true;
}

inline bool readGnss(app::GnssSample& sample) noexcept
{
    sample.time_ms = timeNowMs();
    sample.r_eci_m = {0.0F, 0.0F, 0.0F};
    sample.v_eci_mps = {0.0F, 0.0F, 0.0F};
    sample.valid = false;
    return false;
}

inline bool writeWheelTorque(const app::WheelTorqueCommand& command) noexcept
{
    (void)command;
    return true;
}

inline bool writeMagnetorquer(const app::MagCommand& command) noexcept
{
    (void)command;
    return true;
}

inline bool sendTelemetry(const app::AdcsSnapshot& snapshot, const app::FaultStatus& fault) noexcept
{
    (void)snapshot;
    (void)fault;
    return true;
}

inline void safeAllActuatorsOff() noexcept
{
}
}
