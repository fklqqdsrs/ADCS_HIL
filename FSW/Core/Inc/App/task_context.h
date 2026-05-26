#pragma once

#include "App/messages.h"
#include "App/static_queue.h"
#include "Config/adcs_config.h"

namespace adcs::app
{
struct TaskContext final
{
    StaticQueue<ImuSample, cfg::kImuQueueLength>* imu_queue;
    StaticQueue<MagSample, 4U>* mag_queue;
    StaticQueue<StarTrackerSample, cfg::kStarTrackerQueueLength>* star_tracker_queue;
    StaticQueue<GnssSample, 2U>* gnss_queue;
    StaticQueue<TargetAttitude, 2U>* target_queue;
    StaticQueue<BodyTorqueCommand, cfg::kCommandQueueLength>* body_torque_queue;
    StaticQueue<WheelTorqueCommand, cfg::kCommandQueueLength>* wheel_queue;
    StaticQueue<MagCommand, cfg::kCommandQueueLength>* mag_command_queue;
};

namespace detail
{
extern StaticQueue<ImuSample, cfg::kImuQueueLength> g_imu_queue;
extern StaticQueue<MagSample, 4U> g_mag_queue;
extern StaticQueue<StarTrackerSample, cfg::kStarTrackerQueueLength> g_star_tracker_queue;
extern StaticQueue<GnssSample, 2U> g_gnss_queue;
extern StaticQueue<TargetAttitude, 2U> g_target_queue;
extern StaticQueue<BodyTorqueCommand, cfg::kCommandQueueLength> g_body_torque_queue;
extern StaticQueue<WheelTorqueCommand, cfg::kCommandQueueLength> g_wheel_queue;
extern StaticQueue<MagCommand, cfg::kCommandQueueLength> g_mag_command_queue;

extern TaskContext g_context;
}

inline TaskContext& taskContext() noexcept
{
    return detail::g_context;
}
}
