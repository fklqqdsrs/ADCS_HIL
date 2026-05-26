#pragma once

#include "FreeRTOS.h"
#include "queue.h"
#include <cstddef>
#include <cstdint>

namespace adcs::app
{
template <typename T, std::uint32_t Length>
class StaticQueue final
{
public:
    StaticQueue() = default;
    StaticQueue(const StaticQueue&) = delete;
    StaticQueue& operator=(const StaticQueue&) = delete;
    StaticQueue(StaticQueue&&) = delete;
    StaticQueue& operator=(StaticQueue&&) = delete;

    bool init() noexcept
    {
        handle_ = xQueueCreateStatic(Length,
                                     static_cast<UBaseType_t>(sizeof(T)),
                                     storage_,
                                     &control_block_);
        return handle_ != nullptr;
    }

    bool send(const T& item, const TickType_t timeout_ticks) noexcept
    {
        return xQueueSend(handle_, static_cast<const void*>(&item), timeout_ticks) == pdPASS;
    }

    bool sendFromIsr(const T& item, BaseType_t* const higher_priority_task_woken) noexcept
    {
        return xQueueSendFromISR(handle_, static_cast<const void*>(&item), higher_priority_task_woken) == pdPASS;
    }

    bool receive(T& item, const TickType_t timeout_ticks) noexcept
    {
        return xQueueReceive(handle_, static_cast<void*>(&item), timeout_ticks) == pdPASS;
    }

    bool peek(T& item, const TickType_t timeout_ticks) noexcept
    {
        return xQueuePeek(handle_, static_cast<void*>(&item), timeout_ticks) == pdPASS;
    }

    QueueHandle_t native() const noexcept
    {
        return handle_;
    }

private:
    StaticQueue_t control_block_{};
    alignas(T) std::uint8_t storage_[Length * sizeof(T)]{};
    QueueHandle_t handle_{nullptr};
};
}
