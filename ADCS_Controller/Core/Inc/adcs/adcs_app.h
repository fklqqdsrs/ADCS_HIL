#ifndef ADCS_APP_H
#define ADCS_APP_H

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "event_groups.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

#include "adcs/adcs_config.h"
#include "adcs/adcs_controller.h"
#include "adcs/adcs_platform.h"
#include "adcs/adcs_telemetry.h"
#include "adcs/adcs_types.h"

namespace adcs {
namespace app {
namespace detail {

constexpr uint32_t StackWordsFromBytes(const uint32_t stack_bytes) {
  return stack_bytes / static_cast<uint32_t>(sizeof(StackType_t));
}

static_assert((config::kSensorTaskStackBytes % sizeof(StackType_t)) == 0U, "Sensor stack must be word aligned.");
static_assert((config::kNavigationTaskStackBytes % sizeof(StackType_t)) == 0U, "Navigation stack must be word aligned.");
static_assert((config::kControlTaskStackBytes % sizeof(StackType_t)) == 0U, "Control stack must be word aligned.");
static_assert((config::kTelemetryTaskStackBytes % sizeof(StackType_t)) == 0U, "Telemetry stack must be word aligned.");
static_assert((config::kHealthTaskStackBytes % sizeof(StackType_t)) == 0U, "Health stack must be word aligned.");

struct AppContext {
  osMessageQueueId_t sensor_queue;
  osMessageQueueId_t navigation_queue;
  osMutexId_t state_lock;
  osEventFlagsId_t event_flags;
  TelemetryLogger telemetry;
  Controller controller;
  SharedState state;
  StaticQueue_t sensor_queue_cb;
  StaticQueue_t navigation_queue_cb;
  alignas(uint32_t) uint8_t sensor_queue_storage[config::kQueueDepth * sizeof(SensorSample)];
  alignas(uint32_t) uint8_t navigation_queue_storage[config::kQueueDepth * sizeof(NavigationSolution)];
  StaticSemaphore_t state_lock_cb;
  StaticEventGroup_t event_flags_cb;
  StaticTask_t sensor_task_cb;
  StaticTask_t navigation_task_cb;
  StaticTask_t control_task_cb;
  StaticTask_t telemetry_task_cb;
  StaticTask_t health_task_cb;
  StackType_t sensor_task_stack[StackWordsFromBytes(config::kSensorTaskStackBytes)];
  StackType_t navigation_task_stack[StackWordsFromBytes(config::kNavigationTaskStackBytes)];
  StackType_t control_task_stack[StackWordsFromBytes(config::kControlTaskStackBytes)];
  StackType_t telemetry_task_stack[StackWordsFromBytes(config::kTelemetryTaskStackBytes)];
  StackType_t health_task_stack[StackWordsFromBytes(config::kHealthTaskStackBytes)];
  osThreadId_t sensor_task_handle;
  osThreadId_t navigation_task_handle;
  osThreadId_t control_task_handle;
  osThreadId_t telemetry_task_handle;
  osThreadId_t health_task_handle;
};

inline AppContext &GetApp() {
  static AppContext app = {};
  return app;
}

inline osThreadAttr_t MakeThreadAttributes(
    const char *name,
    void *control_block,
    const uint32_t control_block_bytes,
    StackType_t *stack,
    const uint32_t stack_bytes,
    const osPriority_t priority) {
  osThreadAttr_t attributes = {};
  attributes.name = name;
  attributes.attr_bits = osThreadDetached;
  attributes.cb_mem = control_block;
  attributes.cb_size = control_block_bytes;
  attributes.stack_mem = stack;
  attributes.stack_size = stack_bytes;
  attributes.priority = priority;
  return attributes;
}

inline void UpdateHeartbeat(TaskHeartbeat &heartbeat, const uint32_t tick_ms) {
  heartbeat.last_tick_ms = tick_ms;
  heartbeat.cycle_count += 1U;
}

inline bool LockState() {
  return osMutexAcquire(GetApp().state_lock, config::kStateLockTimeoutMs) == osOK;
}

inline void UnlockState() {
  (void)osMutexRelease(GetApp().state_lock);
}

inline bool ShouldRunDetumble(const Mode previous_mode, const Vector3f &body_rate_rad_s) {
  const float rate_norm_rad_s = body_rate_rad_s.norm();
  if (rate_norm_rad_s >= config::kDetumbleRateThresholdRadPerSec) {
    return true;
  }

  return (previous_mode == Mode::Detumble) &&
         (rate_norm_rad_s >= config::kDetumbleExitRateThresholdRadPerSec);
}

inline Mode DetermineMode(
    const SensorSample &sample,
    const Vector3f &body_rate_rad_s,
    const Mode previous_mode) {
  if (!sample.sensor_ok) {
    return Mode::Fault;
  }
  if (sample.safe_request) {
    return Mode::Safe;
  }
  if ((config::kStartupModeDurationMs > 0U) &&
      (platform::GetTickMs() < config::kStartupModeDurationMs)) {
    return Mode::Startup;
  }
  if (ShouldRunDetumble(previous_mode, body_rate_rad_s)) {
    return Mode::Detumble;
  }
  return Mode::Safe;
}

inline Vector3f FilterRateEstimate(const Vector3f &previous, const Vector3f &measurement) {
  const float alpha = config::kNavigationFilterAlpha;
  return previous + alpha * (measurement - previous);
}

inline SensorSample CreateSensorSample(const uint32_t sequence) {
  const uint32_t tick_ms = platform::GetTickMs();
  SensorSample sample = {};
  sample.sequence = sequence;
  sample.timestamp_ms = tick_ms;
  sample.magnetic_field_body_t = MakeVector3f(0.0F, 0.0F, 0.0F);
  sample.body_rate_rad_s = MakeVector3f(0.0F, 0.0F, 0.0F);
  sample.sun_vector_body_t = MakeVector3f(0.0F, 0.0F, 0.0F);
  sample.safe_request = platform::IsSafeRequested();
  sample.sensor_ok = false;
  return sample;
}

inline NavigationSolution BuildNavigationSolution(
    const SensorSample &sample,
    const Vector3f previous_rate_estimate,
    const Mode previous_mode) {
  const Vector3f filtered_rate_estimate =
      FilterRateEstimate(previous_rate_estimate, sample.body_rate_rad_s);
  NavigationSolution solution = {};
  solution.sequence = sample.sequence;
  solution.timestamp_ms = sample.timestamp_ms;
  solution.mode = DetermineMode(sample, filtered_rate_estimate, previous_mode);
  solution.magnetic_field_body_t = sample.magnetic_field_body_t;
  solution.magnetic_field_rate_body_t_s = MakeVector3f(0.0F, 0.0F, 0.0F);
  solution.body_rate_rad_s = filtered_rate_estimate;

  if (solution.mode == Mode::SunPoint) {
    solution.attitude_error = MakeVector3f(0.020F, -0.012F, 0.006F);
  } else {
    solution.attitude_error = MakeVector3f(0.0F, 0.0F, 0.0F);
  }

  return solution;
}

inline void RegisterQueueOverrun(const uint32_t tick_ms) {
  if (!LockState()) {
    return;
  }

  AppContext &app = GetApp();
  app.state.health.queue_overruns += 1U;
  app.state.health.fault_active = true;
  app.state.health.fault_code = FaultCode::QueueOverrun;
  app.state.health.last_fault_tick_ms = tick_ms;
  app.state.mode = Mode::Fault;
  UnlockState();
  (void)osEventFlagsSet(app.event_flags, config::kEventFaultActive);
}

inline bool IsTaskStale(const TaskHeartbeat &heartbeat, const uint32_t now_ms) {
  return (now_ms - heartbeat.last_tick_ms) > config::kPipelineWatchdogTimeoutMs;
}

inline void SensorTask(void *argument) {
  (void)argument;

  AppContext &app = GetApp();
  uint32_t sequence = 0U;
  for (;;) {
    const SensorSample sample = CreateSensorSample(sequence);
    sequence += 1U;
    const uint32_t now_ms = platform::GetTickMs();

    if (osMessageQueuePut(app.sensor_queue, &sample, 0U, 0U) != osOK) {
      RegisterQueueOverrun(now_ms);
    }

    if (LockState()) {
      app.state.sensor = sample;
      app.state.health.safe_requested = sample.safe_request;
      UpdateHeartbeat(app.state.health.sensor_task, now_ms);
      UnlockState();
    }

    osDelay(config::kSensorTaskPeriodMs);
  }
}

inline void NavigationTask(void *argument) {
  (void)argument;

  AppContext &app = GetApp();
  SensorSample sample = {};
  Vector3f previous_rate_estimate = MakeVector3f(0.0F, 0.0F, 0.0F);
  Mode previous_mode = Mode::Startup;

  for (;;) {
    if (osMessageQueueGet(app.sensor_queue, &sample, nullptr, osWaitForever) != osOK) {
      continue;
    }

    const NavigationSolution solution =
        BuildNavigationSolution(sample, previous_rate_estimate, previous_mode);
    previous_rate_estimate = solution.body_rate_rad_s;
    previous_mode = solution.mode;
    const uint32_t now_ms = platform::GetTickMs();

    if (LockState()) {
      app.state.mode = solution.mode;
      app.state.navigation = solution;
      UpdateHeartbeat(app.state.health.navigation_task, now_ms);
      UnlockState();
    }

    if (osMessageQueuePut(app.navigation_queue, &solution, 0U, 0U) != osOK) {
      RegisterQueueOverrun(now_ms);
    }
  }
}

inline void ControlTask(void *argument) {
  (void)argument;

  AppContext &app = GetApp();
  NavigationSolution latest_solution = {};
  bool has_solution = false;
  uint32_t last_control_tick_ms = platform::GetTickMs();

  for (;;) {
    NavigationSolution pending_solution = {};
    if (osMessageQueueGet(
            app.navigation_queue,
            &pending_solution,
            nullptr,
            config::kSensorTaskPeriodMs) == osOK) {
      latest_solution = pending_solution;
      has_solution = true;

      while (osMessageQueueGet(app.navigation_queue, &pending_solution, nullptr, 0U) == osOK) {
        latest_solution = pending_solution;
      }
    }

    const uint32_t now_ms = platform::GetTickMs();
    if (!has_solution || ((now_ms - last_control_tick_ms) < config::kControlTaskPeriodMs)) {
      continue;
    }

    do {
      last_control_tick_ms += config::kControlTaskPeriodMs;
    } while ((now_ms - last_control_tick_ms) >= config::kControlTaskPeriodMs);
    NavigationSolution solution = latest_solution;

    if (LockState()) {
      if (app.state.health.fault_active) {
        solution.mode = Mode::Fault;
      } else if (app.state.health.safe_requested) {
        solution.mode = Mode::Safe;
      }
      UnlockState();
    }

    const ActuatorCommand command = controller::Compute(app.controller, solution);

    if (LockState()) {
      app.state.mode = command.mode;
      app.state.actuator = command;
      UpdateHeartbeat(app.state.health.control_task, now_ms);
      UnlockState();
    }

    (void)osEventFlagsSet(app.event_flags, config::kEventControlUpdated);
  }
}

inline void TelemetryTask(void *argument) {
  (void)argument;

  AppContext &app = GetApp();
  (void)telemetry::LogBanner(app.telemetry);

  for (;;) {
    const uint32_t flags = osEventFlagsWait(
        app.event_flags,
        config::kEventControlUpdated | config::kEventFaultActive |
            config::kEventSafeRequested,
        osFlagsWaitAny,
        config::kTelemetryTaskPeriodMs);

    const uint32_t now_ms = platform::GetTickMs();
    const uint32_t emitted_flags = ((flags & osFlagsError) != 0U) ? 0U : flags;
    SharedState snapshot = {};

    if (LockState()) {
      UpdateHeartbeat(app.state.health.telemetry_task, now_ms);
      snapshot = app.state;
      UnlockState();
    }

    (void)telemetry::LogSnapshot(app.telemetry, snapshot, emitted_flags);
  }
}

inline void HealthTask(void *argument) {
  (void)argument;

  AppContext &app = GetApp();

  for (;;) {
    const uint32_t now_ms = platform::GetTickMs();
    uint32_t event_bits = 0U;

    if (LockState()) {
      FaultCode fault_code = FaultCode::None;
      bool fault_active = false;

      if (!app.state.sensor.sensor_ok) {
        fault_active = true;
        fault_code = FaultCode::SensorUnavailable;
      } else if (IsTaskStale(app.state.health.sensor_task, now_ms)) {
        fault_active = true;
        fault_code = FaultCode::SensorTaskStale;
      } else if (IsTaskStale(app.state.health.navigation_task, now_ms)) {
        fault_active = true;
        fault_code = FaultCode::NavigationTaskStale;
      } else if (IsTaskStale(app.state.health.control_task, now_ms)) {
        fault_active = true;
        fault_code = FaultCode::ControlTaskStale;
      }

      app.state.health.fault_active = fault_active;
      app.state.health.fault_code = fault_code;
      if (fault_active) {
        app.state.health.last_fault_tick_ms = now_ms;
        app.state.mode = Mode::Fault;
        event_bits |= config::kEventFaultActive;
      }

      if (app.state.health.safe_requested) {
        event_bits |= config::kEventSafeRequested;
      }

      UpdateHeartbeat(app.state.health.health_task, now_ms);
      UnlockState();
    }

    if (event_bits != 0U) {
      (void)osEventFlagsSet(app.event_flags, event_bits);
    } else {
      (void)osEventFlagsClear(app.event_flags, config::kEventFaultActive | config::kEventSafeRequested);
    }

    osDelay(config::kHealthTaskPeriodMs);
  }
}

inline bool CreateRtosObjects() {
  AppContext &app = GetApp();
  const osMessageQueueAttr_t sensor_queue_attributes = {
      "ADCS_SENSOR_Q",
      0U,
      &app.sensor_queue_cb,
      static_cast<uint32_t>(sizeof(app.sensor_queue_cb)),
      app.sensor_queue_storage,
      static_cast<uint32_t>(sizeof(app.sensor_queue_storage)),
  };
  const osMessageQueueAttr_t navigation_queue_attributes = {
      "ADCS_NAV_Q",
      0U,
      &app.navigation_queue_cb,
      static_cast<uint32_t>(sizeof(app.navigation_queue_cb)),
      app.navigation_queue_storage,
      static_cast<uint32_t>(sizeof(app.navigation_queue_storage)),
  };
  const osMutexAttr_t state_lock_attributes = {
      "ADCS_STATE",
      osMutexPrioInherit,
      &app.state_lock_cb,
      static_cast<uint32_t>(sizeof(app.state_lock_cb)),
  };
  const osEventFlagsAttr_t event_flags_attributes = {
      "ADCS_EVENTS",
      0U,
      &app.event_flags_cb,
      static_cast<uint32_t>(sizeof(app.event_flags_cb)),
  };

  app.sensor_queue = osMessageQueueNew(config::kQueueDepth, static_cast<uint32_t>(sizeof(SensorSample)), &sensor_queue_attributes);
  app.navigation_queue = osMessageQueueNew(config::kQueueDepth, static_cast<uint32_t>(sizeof(NavigationSolution)), &navigation_queue_attributes);
  app.state_lock = osMutexNew(&state_lock_attributes);
  app.event_flags = osEventFlagsNew(&event_flags_attributes);

  return (app.sensor_queue != nullptr) && (app.navigation_queue != nullptr) && (app.state_lock != nullptr) &&
         (app.event_flags != nullptr);
}

inline bool CreateTasks() {
  AppContext &app = GetApp();
  const osThreadAttr_t sensor_task_attributes = MakeThreadAttributes(
      "ADCS_SENSOR",
      &app.sensor_task_cb,
      static_cast<uint32_t>(sizeof(app.sensor_task_cb)),
      app.sensor_task_stack,
      static_cast<uint32_t>(sizeof(app.sensor_task_stack)),
      config::kSensorTaskPriority);
  const osThreadAttr_t navigation_task_attributes = MakeThreadAttributes(
      "ADCS_NAV",
      &app.navigation_task_cb,
      static_cast<uint32_t>(sizeof(app.navigation_task_cb)),
      app.navigation_task_stack,
      static_cast<uint32_t>(sizeof(app.navigation_task_stack)),
      config::kNavigationTaskPriority);
  const osThreadAttr_t control_task_attributes = MakeThreadAttributes(
      "ADCS_CTRL",
      &app.control_task_cb,
      static_cast<uint32_t>(sizeof(app.control_task_cb)),
      app.control_task_stack,
      static_cast<uint32_t>(sizeof(app.control_task_stack)),
      config::kControlTaskPriority);
  const osThreadAttr_t telemetry_task_attributes = MakeThreadAttributes(
      "ADCS_TLM",
      &app.telemetry_task_cb,
      static_cast<uint32_t>(sizeof(app.telemetry_task_cb)),
      app.telemetry_task_stack,
      static_cast<uint32_t>(sizeof(app.telemetry_task_stack)),
      config::kTelemetryTaskPriority);
  const osThreadAttr_t health_task_attributes = MakeThreadAttributes(
      "ADCS_HEALTH",
      &app.health_task_cb,
      static_cast<uint32_t>(sizeof(app.health_task_cb)),
      app.health_task_stack,
      static_cast<uint32_t>(sizeof(app.health_task_stack)),
      config::kHealthTaskPriority);

  app.sensor_task_handle = osThreadNew(SensorTask, nullptr, &sensor_task_attributes);
  app.navigation_task_handle = osThreadNew(NavigationTask, nullptr, &navigation_task_attributes);
  app.control_task_handle = osThreadNew(ControlTask, nullptr, &control_task_attributes);
  app.telemetry_task_handle = osThreadNew(TelemetryTask, nullptr, &telemetry_task_attributes);
  app.health_task_handle = osThreadNew(HealthTask, nullptr, &health_task_attributes);

  return (app.sensor_task_handle != nullptr) && (app.navigation_task_handle != nullptr) &&
         (app.control_task_handle != nullptr) && (app.telemetry_task_handle != nullptr) &&
         (app.health_task_handle != nullptr);
}

inline void InitializeState() {
  AppContext &app = GetApp();
  const uint32_t now_ms = platform::GetTickMs();
  app.state = {};
  app.state.mode = Mode::Startup;
  app.state.sensor.magnetic_field_body_t = MakeVector3f(0.0F, 0.0F, 0.0F);
  app.state.sensor.body_rate_rad_s = MakeVector3f(0.0F, 0.0F, 0.0F);
  app.state.sensor.sun_vector_body_t = MakeVector3f(0.0F, 0.0F, 0.0F);
  app.state.navigation.mode = Mode::Startup;
  app.state.navigation.magnetic_field_body_t = MakeVector3f(0.0F, 0.0F, 0.0F);
  app.state.navigation.magnetic_field_rate_body_t_s = MakeVector3f(0.0F, 0.0F, 0.0F);
  app.state.navigation.body_rate_rad_s = MakeVector3f(0.0F, 0.0F, 0.0F);
  app.state.navigation.attitude_error = MakeVector3f(0.0F, 0.0F, 0.0F);
  app.state.actuator.mode = Mode::Startup;
  app.state.actuator.torque_cmd_nm = MakeVector3f(0.0F, 0.0F, 0.0F);
  app.state.actuator.dipole_cmd_a_m2 = MakeVector3f(0.0F, 0.0F, 0.0F);
  app.state.health.fault_code = FaultCode::None;
  app.state.health.sensor_task.last_tick_ms = now_ms;
  app.state.health.navigation_task.last_tick_ms = now_ms;
  app.state.health.control_task.last_tick_ms = now_ms;
  app.state.health.telemetry_task.last_tick_ms = now_ms;
  app.state.health.health_task.last_tick_ms = now_ms;
}

}  // namespace detail

inline bool Initialize() {
  controller::Initialize(detail::GetApp().controller);

  if (!detail::CreateRtosObjects()) {
    return false;
  }

  if (!platform::InitializeUartTx()) {
    return false;
  }

  if (!telemetry::Initialize(detail::GetApp().telemetry)) {
    return false;
  }

  detail::InitializeState();
  return detail::CreateTasks();
}

}  // namespace app
}  // namespace adcs

#endif  // ADCS_APP_H
