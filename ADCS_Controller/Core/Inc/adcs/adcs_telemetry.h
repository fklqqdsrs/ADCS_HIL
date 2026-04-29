#ifndef ADCS_TELEMETRY_H
#define ADCS_TELEMETRY_H

#include <cstdio>

#include "cmsis_os.h"
#include "semphr.h"

#include "adcs/adcs_config.h"
#include "adcs/adcs_platform.h"
#include "adcs/adcs_types.h"

namespace adcs {

struct TelemetryLogger {
  osMutexId_t lock;
  StaticSemaphore_t lock_cb;
};

namespace telemetry {

inline bool Initialize(TelemetryLogger &logger) {
  const osMutexAttr_t attributes = {
      "ADCS_TLM_LOCK",
      osMutexPrioInherit,
      &logger.lock_cb,
      static_cast<uint32_t>(sizeof(logger.lock_cb)),
  };

  logger.lock = osMutexNew(&attributes);
  return logger.lock != nullptr;
}

inline bool WriteLocked(TelemetryLogger &logger, const char *line) {
  if ((logger.lock == nullptr) || (line == nullptr)) {
    return false;
  }

  if (osMutexAcquire(logger.lock, config::kStateLockTimeoutMs) != osOK) {
    return false;
  }

  const bool result = platform::WriteTelemetry(line);
  (void)osMutexRelease(logger.lock);
  return result;
}

inline bool LogBanner(TelemetryLogger &logger) {
  static const char kBanner[] =
      "\r\n[ADCS] Boot complete. Task graph online: SENSOR -> NAV -> CTRL(omega x B / |B| @ 10Hz) -> TLM -> HEALTH\r\n";
  return WriteLocked(logger, kBanner);
}

inline long ToMilliUnits(const float value) {
  return static_cast<long>(value * 1000.0F);
}

inline long ToMicroUnits(const float value) {
  return static_cast<long>(value * 1000000.0F);
}

inline bool LogSnapshot(
    TelemetryLogger &logger,
    const SharedState &state,
    const uint32_t event_flags) {
  char line[config::kTelemetryLineCapacity] = {0};
  const int written = std::snprintf(
      line,
      sizeof(line),
      "[ADCS] seq=%lu t=%lu mode=%s evt=0x%02lX rate_mrad=[%ld,%ld,%ld] mag_uT=[%ld,%ld,%ld] dipole_mA2=[%ld,%ld,%ld] fault=%s\r\n",
      static_cast<unsigned long>(state.navigation.sequence),
      static_cast<unsigned long>(state.navigation.timestamp_ms),
      ToString(state.mode),
      static_cast<unsigned long>(event_flags),
      ToMilliUnits(state.navigation.body_rate_rad_s.x()),
      ToMilliUnits(state.navigation.body_rate_rad_s.y()),
      ToMilliUnits(state.navigation.body_rate_rad_s.z()),
      ToMicroUnits(state.navigation.magnetic_field_body_t.x()),
      ToMicroUnits(state.navigation.magnetic_field_body_t.y()),
      ToMicroUnits(state.navigation.magnetic_field_body_t.z()),
      ToMilliUnits(state.actuator.dipole_cmd_a_m2.x()),
      ToMilliUnits(state.actuator.dipole_cmd_a_m2.y()),
      ToMilliUnits(state.actuator.dipole_cmd_a_m2.z()),
      ToString(state.health.fault_code));

  if ((written <= 0) || (static_cast<size_t>(written) >= sizeof(line))) {
    return false;
  }

  return WriteLocked(logger, line);
}

}  // namespace telemetry
}  // namespace adcs

#endif  // ADCS_TELEMETRY_H
