#pragma once

#include "Config/adcs_config.h"
#include "Bsp/board_io.h"
#include "Estimation/mekf.h"
#include "Control/quat_pd_controller.h"
#include "App/shared_state.h"
#include "App/task_context.h"
#include "Actuation/wheel_allocator.h"
#include "FreeRTOS.h"
#include "task.h"
#include <cstdint>

namespace adcs::app
{
namespace detail
{   
inline TargetAttitude defaultTarget(const std::uint64_t time_ms) noexcept
{

    return TargetAttitude{time_ms,
                          {1.0F, 0.0F, 0.0F, 0.0F},
                          {0.0F, 0.0F, 0.0F},
                          {0.0F, 0.0F, 0.0F},
                          true};
}

}

inline void sensorTask(void* argument) noexcept
{
    (void)argument;
    TaskContext& ctx = taskContext();
    TickType_t last_wake = xTaskGetTickCount();
    const TickType_t period = pdMS_TO_TICKS(10U);

    for (;;)
    {
        ImuSample imu{};
        if (bsp::readImu(imu))
        {
            static_cast<void>(ctx.imu_queue->send(imu, 0U));
        }

        MagSample mag{};
        if (bsp::readMag(mag))
        {
            static_cast<void>(ctx.mag_queue->send(mag, 0U));
        }

        GnssSample gnss{};
        if (bsp::readGnss(gnss))
        {
            static_cast<void>(ctx.gnss_queue->send(gnss, 0U));
        }

        vTaskDelayUntil(&last_wake, period);
    }
}

inline void starTrackerTask(void* argument) noexcept
{
    (void)argument;
    TaskContext& ctx = taskContext();
    TickType_t last_wake = xTaskGetTickCount();
    const TickType_t period = pdMS_TO_TICKS(100U);

    for (;;)
    {
        StarTrackerSample st{};
        if (bsp::readStarTracker(st))
        {
            static_cast<void>(ctx.star_tracker_queue->send(st, 0U));
        }
        vTaskDelayUntil(&last_wake, period);
    }
}

inline void estimatorTask(void* argument) noexcept
{
    (void)argument;
    TaskContext& ctx = taskContext();
    est::Mekf estimator{};
    estimator.reset(sharedState().readSnapshot());

    TickType_t last_wake = xTaskGetTickCount();
    const TickType_t period = pdMS_TO_TICKS(10U);

    for (;;)
    {
        ImuSample imu{};
        if (ctx.imu_queue->receive(imu, 0U))
        {
            estimator.propagate(imu, cfg::kEstimatorDtSec);
        }

        StarTrackerSample st{};
        while (ctx.star_tracker_queue->receive(st, 0U))
        {
            estimator.updateStarTracker(st);
        }

        GnssSample gnss{};
        while (ctx.gnss_queue->receive(gnss, 0U))
        {
            estimator.updateGnss(gnss);
        }

        sharedState().writeSnapshot(estimator.snapshot());
        vTaskDelayUntil(&last_wake, period);
    }
}

inline void controlTask(void* argument) noexcept
{
    (void)argument;
    TaskContext& ctx = taskContext();
    ctrl::QuatPdController controller{};
    controller.init();

    TickType_t last_wake = xTaskGetTickCount();
    const TickType_t period = pdMS_TO_TICKS(50U);

    TargetAttitude target = detail::defaultTarget(0U);

    for (;;)
    {
        TargetAttitude new_target{};
        while (ctx.target_queue->receive(new_target, 0U))
        {
            if (new_target.valid)
            {
                target = new_target;
            }
        }

        const AdcsSnapshot snapshot = sharedState().readSnapshot();
        if (target.time_ms == 0U)
        {
            target = detail::defaultTarget(snapshot.time_ms);
        }

        const BodyTorqueCommand body_command = controller.compute(snapshot, target);
        static_cast<void>(ctx.body_torque_queue->send(body_command, 0U));

        vTaskDelayUntil(&last_wake, period);
    }
}

inline void actuatorTask(void* argument) noexcept
{
    (void)argument;
    TaskContext& ctx = taskContext();
    act::WheelAllocator allocator{};
    allocator.init();

    for (;;)
    {
        BodyTorqueCommand body_command{};
        if (ctx.body_torque_queue->receive(body_command, portMAX_DELAY))
        {
            const WheelTorqueCommand wheel_command = allocator.allocate(body_command);
            const MagCommand mag_command = allocator.zeroMagCommand(body_command.time_ms);

            if (wheel_command.valid)
            {
                static_cast<void>(bsp::writeWheelTorque(wheel_command));
                static_cast<void>(ctx.wheel_queue->send(wheel_command, 0U));
            }
            if (mag_command.valid)
            {
                static_cast<void>(bsp::writeMagnetorquer(mag_command));
                static_cast<void>(ctx.mag_command_queue->send(mag_command, 0U));
            }
        }
    }
}

inline void telemetryTask(void* argument) noexcept
{
    (void)argument;
    TickType_t last_wake = xTaskGetTickCount();
    const TickType_t period = pdMS_TO_TICKS(100U);

    for (;;)
    {
        const AdcsSnapshot snapshot = sharedState().readSnapshot();
        const FaultStatus fault = sharedState().readFault();
        static_cast<void>(bsp::sendTelemetry(snapshot, fault));
        vTaskDelayUntil(&last_wake, period);
    }
}

inline void faultMonitorTask(void* argument) noexcept
{
    (void)argument;
    TickType_t last_wake = xTaskGetTickCount();
    const TickType_t period = pdMS_TO_TICKS(1000U);

    for (;;)
    {
        FaultStatus fault = sharedState().readFault();
        const AdcsSnapshot snapshot = sharedState().readSnapshot();

        constexpr std::uint64_t kAttitudeTimeoutMs = 500U;
        const std::uint64_t now = bsp::timeNowMs();
        if ((snapshot.attitude_valid) && ((now - snapshot.time_ms) > kAttitudeTimeoutMs))
        {
            fault.bitmask |= 0x00000001UL;
            bsp::safeAllActuatorsOff();
        }

        sharedState().writeFault(fault);
        bsp::feedWatchdog();
        vTaskDelayUntil(&last_wake, period);
    }
}
}
