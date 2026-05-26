#pragma once

#include "Bsp/board_io.h"
#include "App/shared_state.h"
#include "App/static_task.h"
#include "App/task_context.h"
#include "App/tasks.h"

namespace adcs::app
{
namespace detail
{
extern StaticTask<768U> g_sensor_task;
extern StaticTask<768U> g_star_tracker_task;
extern StaticTask<1408U> g_estimator_task;
extern StaticTask<1024U> g_control_task;
extern StaticTask<768U> g_actuator_task;
extern StaticTask<768U> g_telemetry_task;
extern StaticTask<512U> g_fault_task;

inline bool initQueues(TaskContext& ctx) noexcept
{
    bool ok = true;
    ok = ctx.imu_queue->init() && ok;
    ok = ctx.mag_queue->init() && ok;
    ok = ctx.star_tracker_queue->init() && ok;
    ok = ctx.gnss_queue->init() && ok;
    ok = ctx.target_queue->init() && ok;
    ok = ctx.body_torque_queue->init() && ok;
    ok = ctx.wheel_queue->init() && ok;
    ok = ctx.mag_command_queue->init() && ok;
    return ok;
}
}

inline bool init() noexcept
{
    bsp::boardInit();

    bool ok = sharedState().init();
    TaskContext& ctx = taskContext();
    ok = detail::initQueues(ctx) && ok;

    ok = detail::g_sensor_task.create(sensorTask, "ADCS_SENS", nullptr, 4U) && ok;
    ok = detail::g_star_tracker_task.create(starTrackerTask, "ADCS_ST", nullptr, 4U) && ok;
    ok = detail::g_estimator_task.create(estimatorTask, "ADCS_EST", nullptr, 5U) && ok;
    ok = detail::g_control_task.create(controlTask, "ADCS_CTRL", nullptr, 6U) && ok;
    ok = detail::g_actuator_task.create(actuatorTask, "ADCS_ACT", nullptr, 7U) && ok;
    ok = detail::g_telemetry_task.create(telemetryTask, "ADCS_TLM", nullptr, 2U) && ok;
    ok = detail::g_fault_task.create(faultMonitorTask, "ADCS_FLT", nullptr, 3U) && ok;

    if (!ok)
    {
        bsp::safeAllActuatorsOff();
    }
    return ok;
}
}
