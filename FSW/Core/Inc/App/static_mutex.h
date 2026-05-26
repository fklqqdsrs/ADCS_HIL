#pragma once

#include "FreeRTOS.h"
#include "semphr.h"

namespace adcs::app
{
class StaticMutex final
{
public:
    StaticMutex() = default;
    StaticMutex(const StaticMutex&) = delete;
    StaticMutex& operator=(const StaticMutex&) = delete;
    StaticMutex(StaticMutex&&) = delete;
    StaticMutex& operator=(StaticMutex&&) = delete;

    bool init() noexcept
    {
        handle_ = xSemaphoreCreateMutexStatic(&control_block_);
        return handle_ != nullptr;
    }

    bool lock(const TickType_t timeout_ticks) noexcept
    {
        return xSemaphoreTake(handle_, timeout_ticks) == pdPASS;
    }

    void unlock() noexcept
    {
        static_cast<void>(xSemaphoreGive(handle_));
    }

private:
    StaticSemaphore_t control_block_{};
    SemaphoreHandle_t handle_{nullptr};
};

class MutexGuard final
{
public:
    explicit MutexGuard(StaticMutex& mutex) noexcept : mutex_{mutex}, locked_{mutex.lock(portMAX_DELAY)} {}
    ~MutexGuard() noexcept
    {
        if (locked_)
        {
            mutex_.unlock();
        }
    }

    MutexGuard(const MutexGuard&) = delete;
    MutexGuard& operator=(const MutexGuard&) = delete;
    MutexGuard(MutexGuard&&) = delete;
    MutexGuard& operator=(MutexGuard&&) = delete;

    bool locked() const noexcept
    {
        return locked_;
    }

private:
    StaticMutex& mutex_;
    bool locked_;
};
}
