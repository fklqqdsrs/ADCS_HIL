#ifndef ADCS_RATE_DUMPING_H
#define ADCS_RATE_DUMPING_H

#include "adcs/adcs_types.h"

namespace adcs {
namespace rate_dumping {

inline Vector3f LimitCommand(const Vector3f &command, const float limit) {
  if ((!IsFiniteVector(command)) || (!IsFinite(limit)) || (limit <= 0.0F)) {
    return Vector3f::Zero();
  }

  const Vector3f min_limit = Vector3f::Constant(-limit);
  const Vector3f max_limit = Vector3f::Constant(limit);
  return command.cwiseMax(min_limit).cwiseMin(max_limit);
}

inline Vector3f ComputeTorqueCommand(
    const Vector3f &body_rate_rad_s,
    const float gain,
    const float limit_nm) {
  if ((!IsFiniteVector(body_rate_rad_s)) || (!IsFinite(gain)) || (!IsFinite(limit_nm)) ||
      (gain <= 0.0F) || (limit_nm <= 0.0F)) {
    return Vector3f::Zero();
  }

  const Vector3f torque_cmd = -gain * body_rate_rad_s;
  return LimitCommand(torque_cmd, limit_nm);
}

}  // namespace rate_dumping
}  // namespace adcs

#endif  // ADCS_RATE_DUMPING_H
