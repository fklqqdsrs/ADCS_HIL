#pragma once

#include <cstdint>
#include "Math/math_types.h"
#include "Config/compiler_policy.h"

namespace adcs::cfg
{
constexpr float kControlRateHz = 20.0F;
constexpr float kEstimatorRateHz = 100.0F;
constexpr float kSensorRateHz = 100.0F;
constexpr float kStarTrackerRateHz = 10.0F;

constexpr float kControlDtSec = 1.0F / kControlRateHz;
constexpr float kEstimatorDtSec = 1.0F / kEstimatorRateHz;

constexpr float kMaxBodyTorqueNm = 0.020F;
constexpr float kMaxWheelTorqueNm = 0.020F;
constexpr float kMaxMagDipoleAm2 = 10.0F;
constexpr float kWheelMomentumLimitNms = 0.500F;
constexpr float kWheelSpeedLimitRpm = 7200.0F;


/*reaction wheel inertia [kg m^2] */
constexpr float kWheelInertia = 0.000662F;

/* Example inertia in body frame [kg m^2]. Replace with measured CAD/MOI. */
constexpr float kIxx = 8.58173218F;
constexpr float kIxy = 0.12178819F;
constexpr float kIxz = -0.23592313F;
constexpr float kIyy = 8.19663254F;
constexpr float kIyz = -0.25803852F;
constexpr float kIzz = 7.19731638F;



/* Quaternion PD gains. Start conservative and tune in simulation/HIL. */
constexpr float kKpX = 0.004F;
constexpr float kKpY = 0.004F;
constexpr float kKpZ = 0.004F;
constexpr float kKdX = 0.060F;
constexpr float kKdY = 0.060F;
constexpr float kKdZ = 0.060F;

constexpr std::uint32_t kCommandQueueLength = 8U;
constexpr std::uint32_t kImuQueueLength = 8U;
constexpr std::uint32_t kStarTrackerQueueLength = 4U;

/*wheel allocator */
inline math::Mat34 makeWheelAxesBodyMatrix() noexcept
{
    math::Mat34 m;
    m << 0.5773F,  0.5773F,  0.5773F,  0.5773F,
         0.5774F,  0.5774F, -0.5774F, -0.5774F,
         0.5774F, -0.5774F, -0.5774F,  0.5774F;
    return m;
}

inline math::Mat43 makeBodyAxesWheelMatrix() noexcept
{
    math::Mat43 m;
    m <<  0.4330F,  0.4330F,  0.4330F,
          0.4330F,  0.4330F, -0.4330F,
          0.4330F, -0.4330F, -0.4330F,
          0.4330F, -0.4330F,  0.4330F;
    return m;
}

}
