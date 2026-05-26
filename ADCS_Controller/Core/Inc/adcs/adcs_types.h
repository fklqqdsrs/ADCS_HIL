#ifndef ADCS_TYPES_H
#define ADCS_TYPES_H

#include <cstdint>
#include <cmath>

#include <eigen-3.4.0/Eigen/Dense>

namespace adcs {

enum class Mode : uint8_t {
  Startup = 0U,
  Detumble,
  SunPoint,
  Safe,
  Fault,
};

enum class FaultCode : uint8_t {
  None = 0U,
  QueueOverrun,
  SensorUnavailable,
  SensorTaskStale,
  NavigationTaskStale,
  ControlTaskStale,
};

using Vector3f = Eigen::Matrix<float, 3, 1, Eigen::DontAlign>;
using Vector3d = Eigen::Matrix<double,3, 1, Eigen::DontAlign>;

struct SensorSample {
  uint32_t sequence;
  uint32_t timestamp_ms;
  Vector3f magnetic_field_body_t;
  Vector3f body_rate_rad_s;
  Vector3f sun_vector_body_t;
  bool safe_request;
  bool sensor_ok;
};

struct NavigationSolution {
  uint32_t sequence;
  uint32_t timestamp_ms;
  Mode mode;
  Vector3f magnetic_field_body_t;
  Vector3f magnetic_field_rate_body_t_s;
  Vector3f body_rate_rad_s;
  Vector3f attitude_error;
};

struct ActuatorCommand {
  uint32_t sequence;
  uint32_t timestamp_ms;
  Mode mode;
  Vector3f torque_cmd_nm;
  Vector3f dipole_cmd_a_m2;
};

struct TaskHeartbeat {
  uint32_t last_tick_ms;
  uint32_t cycle_count;
};

struct HealthStatus {
  bool fault_active;
  bool safe_requested;
  FaultCode fault_code;
  uint32_t queue_overruns;
  uint32_t last_fault_tick_ms;
  TaskHeartbeat sensor_task;
  TaskHeartbeat navigation_task;
  TaskHeartbeat control_task;
  TaskHeartbeat telemetry_task;
  TaskHeartbeat health_task;
};

struct SharedState {
  Mode mode;
  SensorSample sensor;
  NavigationSolution navigation;
  ActuatorCommand actuator;
  HealthStatus health;
};

inline Vector3f MakeVector3f(const float x, const float y, const float z) {
  return Vector3f(x, y, z);
}

inline Vector3d MakeVector3d(const double x, const double y, const double z) {
  return Vector3d(x, y, z);
}

inline float Absolute(const float value) {
  return (value < 0.0F) ? -value : value;
}

inline float Clamp(const float value, const float min_value, const float max_value) {
  if (value < min_value) {
    return min_value;
  }
  if (value > max_value) {
    return max_value;
  }
  return value;
}

inline bool IsFinite(const float value) {
  return std::isfinite(value);
}

inline bool IsFiniteVector(const Vector3f &vector) {
  return IsFinite(vector.x()) && IsFinite(vector.y()) && IsFinite(vector.z());
}

inline const char *ToString(const Mode mode) {
  switch (mode) {
    case Mode::Startup:
      return "STARTUP";
    case Mode::Detumble:
      return "DETUMBLE";
    case Mode::SunPoint:
      return "SUN_POINT";
    case Mode::Safe:
      return "SAFE";
    case Mode::Fault:
      return "FAULT";
    default:
      return "UNKNOWN";
  }
}

inline const char *ToString(const FaultCode fault_code) {
  switch (fault_code) {
    case FaultCode::None:
      return "NONE";
    case FaultCode::QueueOverrun:
      return "QUEUE_OVERRUN";
    case FaultCode::SensorUnavailable:
      return "SENSOR_UNAVAILABLE";
    case FaultCode::SensorTaskStale:
      return "SENSOR_STALE";
    case FaultCode::NavigationTaskStale:
      return "NAV_STALE";
    case FaultCode::ControlTaskStale:
      return "CTRL_STALE";
    default:
      return "UNKNOWN";
  }
}

inline float MaxAbsComponent(const Vector3f &vector) {
  return vector.cwiseAbs().maxCoeff();
}

}  // namespace adcs

#endif  // ADCS_TYPES_H
