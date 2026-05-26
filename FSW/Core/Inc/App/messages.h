#pragma once

#include "Math/math_types.h"
#include <cstdint>

namespace adcs::app
{
enum class AdcsMode : std::uint8_t
{
    Boot = 0U,
    Safe = 1U,
    Detumble = 2U,
    SunPoint = 3U,
    Nominal = 4U,
    Imaging = 5U,
	LaserRanging = 6U,
	SpaceWeather = 7U,
    Fault = 255U
};

struct ImuSample final
{
    std::uint64_t time_ms;
    math::RawVec3 gyro_radps;
    math::RawVec3 accel_mps2;
    bool valid;
};

struct MagSample final
{
    std::uint64_t time_ms;
    math::RawVec3 mag_tesla;
    bool valid;
};

struct StarTrackerSample final
{
    std::uint64_t time_ms;
    math::RawQuat q_body_to_eci;
    math::RawVec3 omega_body_radps;
    float attitude_sigma_rad;
    bool valid;
};

struct GnssSample final
{
    std::uint64_t time_ms;
    math::RawVec3 r_eci_m;
    math::RawVec3 v_eci_mps;
    bool valid;
};

struct OrbitPropogator final
{
	std::uint64_t time_ms;
	math::RawVec3 r_eci_m;
	math::RawVec3 v_eci_mps;
	bool valid;
};

struct SunVectorsample final
{
	std::uint64_t time_ms;
	math::RawVec3 sun_vector;
	bool valid;
};

struct SunEphemeris final
{
	std::uint64_t time_ms;
	math::RawVec3 sun_vector;
	bool valid;
};

struct TargetAttitude final
{
    std::uint64_t time_ms;
    math::RawQuat q_target_to_eci;
    math::RawVec3 omega_target_body_radps;
    math::RawVec3 omega_dot_target_body_radps2;
    bool valid;
};

struct BodyTorqueCommand final
{
    std::uint64_t time_ms;
    math::RawVec3 torque_body_nm;
    bool valid;
};

struct WheelTorqueCommand final
{
    std::uint64_t time_ms;
    float wheel_torque_nm[4];
    bool valid;
};

struct WheelSpeedCommand final
{
    std::uint64_t time_ms;
    std::int32_t wheel_speed_mrpm[4];
    float dt;
    bool valid;
};

struct MagCommand final
{
    std::uint64_t time_ms;
    math::RawVec3 dipole_body_am2;
    bool valid;
};

struct AdcsSnapshot final
{
    std::uint64_t time_ms;
    AdcsMode mode;
    math::RawQuat q_body_to_eci;
    math::RawVec3 omega_body_radps;
    math::RawVec3 r_eci_m;
    math::RawVec3 v_eci_mps;
    bool attitude_valid;
    bool orbit_valid;
};

struct FaultStatus final
{
    std::uint32_t bitmask;
};
}
