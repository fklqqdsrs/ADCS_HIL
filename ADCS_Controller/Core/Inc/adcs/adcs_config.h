#ifndef ADCS_CONFIG_H
#define ADCS_CONFIG_H

#include <cstdint>

#include "cmsis_os.h"

namespace adcs {
namespace config {

constexpr uint32_t kSensorTaskPeriodMs = 100U;
constexpr uint32_t kControlTaskPeriodMs = 100U;
constexpr uint32_t kTelemetryTaskPeriodMs = 1000U;
constexpr uint32_t kHealthTaskPeriodMs = 250U;
constexpr uint32_t kPipelineWatchdogTimeoutMs = 500U;
constexpr uint32_t kStateLockTimeoutMs = 25U;
constexpr uint32_t kQueueDepth = 32U;
constexpr uint32_t kUartTimeoutMs = 25U;
constexpr uint32_t kStartupModeDurationMs = 0U;
constexpr uint32_t kTelemetryLineCapacity = 200U;
constexpr uint32_t kUartTxBufferCapacity = kTelemetryLineCapacity;
constexpr bool kEnableTextTelemetry = true;

constexpr uint32_t kSensorTaskStackBytes = 512U * 4U;
constexpr uint32_t kNavigationTaskStackBytes = 512U * 4U;
constexpr uint32_t kControlTaskStackBytes = 768U * 4U;
constexpr uint32_t kTelemetryTaskStackBytes = 768U * 4U;
constexpr uint32_t kHealthTaskStackBytes = 512U * 4U;

constexpr osPriority_t kSensorTaskPriority = osPriorityHigh;
constexpr osPriority_t kNavigationTaskPriority = osPriorityAboveNormal;
constexpr osPriority_t kControlTaskPriority = osPriorityAboveNormal;
constexpr osPriority_t kTelemetryTaskPriority = osPriorityBelowNormal;
constexpr osPriority_t kHealthTaskPriority = osPriorityNormal;

constexpr float kDetumbleRateThresholdRadPerSec = 0.05F;
constexpr float kDetumbleExitRateThresholdRadPerSec = 0.00349066F;
constexpr float kRateGain = 0.14F;
constexpr float kAttitudeGain = 0.08F;
constexpr float kCommandLimitNm = 29.63F;
constexpr float kDipoleLimitAm2 = 8.0F;
constexpr float kDetumbleDipoleGain = 0.036F;
constexpr float kBdotGain = 0.03F;
constexpr float kMinimumFieldNormTesla = 1.0e-7F;
constexpr float kNavigationFilterAlpha = 0.30F;

constexpr uint32_t kEventControlUpdated = 0x00000001U;
constexpr uint32_t kEventFaultActive = 0x00000002U;
constexpr uint32_t kEventSafeRequested = 0x00000004U;

}  // namespace config
}  // namespace adcs

#endif  // ADCS_CONFIG_H
