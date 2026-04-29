#ifndef ADCS_BDOT_H
#define ADCS_BDOT_H

#include "adcs/adcs_types.h"

namespace adcs {
namespace bdot {

struct State {
  bool is_initialized;
  uint32_t previous_timestamp_ms;
  Vector3f previous_magnetic_field_t;
  Vector3f magnetic_field_rate_t_s;
};

inline void Initialize(State &state) {
  state.is_initialized = false;
  state.previous_timestamp_ms = 0U;
  state.previous_magnetic_field_t = Vector3f::Zero();
  state.magnetic_field_rate_t_s = Vector3f::Zero();
}

inline Vector3f LimitCommand(const Vector3f &command, const float limit) {
  if ((!IsFiniteVector(command)) || (!IsFinite(limit)) || (limit <= 0.0F)) {
    return Vector3f::Zero();
  }

  const Vector3f min_limit = Vector3f::Constant(-limit);
  const Vector3f max_limit = Vector3f::Constant(limit);
  return command.cwiseMax(min_limit).cwiseMin(max_limit);
}

inline Vector3f EstimateFieldRate(
    State &state,
    const Vector3f &magnetic_field_t,
    const uint32_t timestamp_ms) {
  if (!IsFiniteVector(magnetic_field_t)) {
    state.magnetic_field_rate_t_s = Vector3f::Zero();
    return state.magnetic_field_rate_t_s;
  }

  if ((!state.is_initialized) || (timestamp_ms <= state.previous_timestamp_ms)) {
    state.is_initialized = true;
    state.previous_timestamp_ms = timestamp_ms;
    state.previous_magnetic_field_t = magnetic_field_t;
    state.magnetic_field_rate_t_s = Vector3f::Zero();
    return state.magnetic_field_rate_t_s;
  }

  const float dt_s = static_cast<float>(timestamp_ms - state.previous_timestamp_ms) * 0.001F;
  state.magnetic_field_rate_t_s = (magnetic_field_t - state.previous_magnetic_field_t) / dt_s;
  state.previous_timestamp_ms = timestamp_ms;
  state.previous_magnetic_field_t = magnetic_field_t;
  return state.magnetic_field_rate_t_s;
}

inline Vector3f ComputeDipoleCommand(
    const Vector3f &magnetic_field_rate_t_s,
    const float gain,
    const float limit_a_m2) {
  if ((!IsFiniteVector(magnetic_field_rate_t_s)) || (!IsFinite(gain)) || (!IsFinite(limit_a_m2)) ||
      (gain <= 0.0F) || (limit_a_m2 <= 0.0F)) {
    return Vector3f::Zero();
  }

  const Vector3f dipole_cmd = -gain * magnetic_field_rate_t_s;
  return LimitCommand(dipole_cmd, limit_a_m2);
}

}  // namespace bdot
}  // namespace adcs

#endif  // ADCS_BDOT_H
