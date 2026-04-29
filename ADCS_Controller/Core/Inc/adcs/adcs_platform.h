#ifndef ADCS_PLATFORM_H
#define ADCS_PLATFORM_H

#include <array>
#include <cstddef>
#include <cstring>

#include "cmsis_os.h"
#include "semphr.h"
#include "main.h"
#include "usart.h"

#include "adcs/adcs_config.h"
#include "adcs/adcs_types.h"

namespace adcs {
namespace platform {
namespace detail {

struct UartTxContext {
  osMutexId_t lock;
  StaticSemaphore_t lock_cb;
};

inline UartTxContext &GetUartTxContext() {
  static UartTxContext context = {};
  return context;
}

inline bool EnsureUartTxLock() {
  UartTxContext &context = GetUartTxContext();
  if (context.lock != nullptr) {
    return true;
  }

  if (osKernelGetState() == osKernelInactive) {
    return false;
  }

  const osMutexAttr_t attributes = {
      "ADCS_UART_TX",
      osMutexPrioInherit,
      &context.lock_cb,
      static_cast<uint32_t>(sizeof(context.lock_cb)),
  };

  context.lock = osMutexNew(&attributes);
  return context.lock != nullptr;
}

inline size_t StringLengthBounded(const char *text, const size_t capacity) {
  size_t length = 0U;
  if (text == nullptr) {
    return 0U;
  }

  while ((length < capacity) && (text[length] != '\0')) {
    length += 1U;
  }
  return length;
}

}  // namespace detail

inline uint32_t GetTickMs() {
  return HAL_GetTick();
}

inline bool IsSafeRequested() {
  return HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_SET;
}

inline bool InitializeUartTx() {
  return detail::EnsureUartTxLock();
}

inline bool WriteBytes(const uint8_t *data, const uint16_t length) {
  if (length == 0U) {
    return true;
  }

  if ((data == nullptr) || (static_cast<uint32_t>(length) > config::kUartTxBufferCapacity)) {
    return false;
  }

  if (!detail::EnsureUartTxLock()) {
    return false;
  }

  detail::UartTxContext &context = detail::GetUartTxContext();
  if (osMutexAcquire(context.lock, config::kUartTimeoutMs) != osOK) {
    return false;
  }

  std::array<uint8_t, config::kUartTxBufferCapacity> tx_buffer = {};
  std::memcpy(tx_buffer.data(), data, length);

  const bool result =
      HAL_UART_Transmit(&huart3, tx_buffer.data(), length, config::kUartTimeoutMs) == HAL_OK;
  (void)osMutexRelease(context.lock);
  return result;
}

inline bool WriteTelemetry(const char *line) {
  if (!config::kEnableTextTelemetry) {
    return true;
  }

  if (line == nullptr) {
    return false;
  }

  const size_t length = detail::StringLengthBounded(line, config::kUartTxBufferCapacity);
  if (length == 0U) {
    return true;
  }

  if (length >= config::kUartTxBufferCapacity) {
    return false;
  }

  std::array<uint8_t, config::kUartTxBufferCapacity> payload = {};
  for (size_t index = 0U; index < length; ++index) {
    payload[index] = static_cast<uint8_t>(line[index]);
  }

  return WriteBytes(payload.data(), static_cast<uint16_t>(length));
}

}  // namespace platform
}  // namespace adcs

#endif  // ADCS_PLATFORM_H
