#ifndef ADCS_CONTROLLER_H
#define ADCS_CONTROLLER_H

#include "adcs/adcs_config.h"
#include "adcs/adcs_magnetic_detumble.h"
#include "adcs/adcs_types.h"

namespace adcs {

struct Controller {
  float rate_gain;
  float attitude_gain;
  float torque_limit_nm;
  float detumble_dipole_gain;
  float dipole_limit_a_m2;
};

namespace controller {

inline void Initialize(Controller &controller_instance) {
  controller_instance.rate_gain = config::kRateGain;
  controller_instance.attitude_gain = config::kAttitudeGain;
  controller_instance.torque_limit_nm = config::kCommandLimitNm;
  controller_instance.detumble_dipole_gain = config::kDetumbleDipoleGain;
  controller_instance.dipole_limit_a_m2 = config::kDipoleLimitAm2;
}

inline Vector3f LimitVector(const Vector3f &vector, const float limit) {
  if ((!IsFiniteVector(vector)) || (!IsFinite(limit)) || (limit <= 0.0F)) {
    return Vector3f::Zero();
  }

  const Vector3f min_limit = Vector3f::Constant(-limit);
  const Vector3f max_limit = Vector3f::Constant(limit);
  return vector.cwiseMax(min_limit).cwiseMin(max_limit);
}

inline ActuatorCommand Compute(
    Controller &controller_instance,
    const NavigationSolution &navigation_solution) {
  Vector3f torque_cmd = Vector3f::Zero();
  Vector3f dipole_cmd = Vector3f::Zero();

  switch (navigation_solution.mode) {
    case Mode::Detumble:
      dipole_cmd = magnetic_detumble::ComputeDipoleCommand(
          navigation_solution.body_rate_rad_s,
          navigation_solution.magnetic_field_body_t,
          controller_instance.detumble_dipole_gain,
          config::kMinimumFieldNormTesla,
          controller_instance.dipole_limit_a_m2);
      break;
    case Mode::SunPoint:
      if (IsFiniteVector(navigation_solution.body_rate_rad_s) &&
          IsFiniteVector(navigation_solution.attitude_error) &&
          IsFinite(controller_instance.rate_gain) &&
          IsFinite(controller_instance.attitude_gain)) {
        torque_cmd = -controller_instance.rate_gain * navigation_solution.body_rate_rad_s -
                     controller_instance.attitude_gain * navigation_solution.attitude_error;
      }
      break;
    case Mode::Startup:
    case Mode::Safe:
    case Mode::Fault:
    default:
      break;
  }

  ActuatorCommand command = {};
  command.sequence = navigation_solution.sequence;
  command.timestamp_ms = navigation_solution.timestamp_ms;
  command.mode = navigation_solution.mode;
  command.torque_cmd_nm = LimitVector(torque_cmd, controller_instance.torque_limit_nm);
  command.dipole_cmd_a_m2 = LimitVector(dipole_cmd, controller_instance.dipole_limit_a_m2);
  return command;
}

}  // namespace controller
}  // namespace adcs

#endif  // ADCS_CONTROLLER_H
