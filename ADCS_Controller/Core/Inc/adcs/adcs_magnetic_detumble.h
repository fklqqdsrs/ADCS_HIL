#ifndef ADCS_MAGNETIC_DETUMBLE_H
#define ADCS_MAGNETIC_DETUMBLE_H

#include "adcs/adcs_types.h"

namespace adcs {
namespace magnetic_detumble {

inline Vector3f LimitCommand(const Vector3f &command, const float limit) {
  if ((!IsFiniteVector(command)) || (!IsFinite(limit)) || (limit <= 0.0F)) {
    return Vector3f::Zero();
  }

  const Vector3f min_limit = Vector3f::Constant(-limit);
  const Vector3f max_limit = Vector3f::Constant(limit);
  return command.cwiseMax(min_limit).cwiseMin(max_limit);
}

inline Vector3f ComputeDipoleCommand(
    const Vector3f &body_rate_rad_s,
    const Vector3f &magnetic_field_body_t,
    const float gain,
    const float minimum_field_norm_t,
    const float limit_a_m2) {
  if ((!IsFiniteVector(body_rate_rad_s)) || (!IsFiniteVector(magnetic_field_body_t)) ||
      (!IsFinite(gain)) || (!IsFinite(minimum_field_norm_t)) || (!IsFinite(limit_a_m2)) ||
      (gain <= 0.0F) || (minimum_field_norm_t <= 0.0F) || (limit_a_m2 <= 0.0F)) {
    return Vector3f::Zero();
  }

  const float magnetic_field_norm_t = magnetic_field_body_t.norm();
  if ((!IsFinite(magnetic_field_norm_t)) || (magnetic_field_norm_t <= minimum_field_norm_t)) {
    return Vector3f::Zero();
  }

  const Vector3f unit_magnetic_field_body = magnetic_field_body_t / magnetic_field_norm_t;
  const Vector3f dipole_cmd =
      (gain / magnetic_field_norm_t) * (body_rate_rad_s.cross(unit_magnetic_field_body));
  return LimitCommand(dipole_cmd, limit_a_m2);
}

}  // namespace magnetic_detumble
}  // namespace adcs

#endif  // ADCS_MAGNETIC_DETUMBLE_H
