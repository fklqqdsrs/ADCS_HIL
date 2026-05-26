#pragma once

#include "FreeRTOS.h"
#include "task.h"
#include <cstdint>

namespace adcs::app
{
template <std::uint32_t StackWords>
class StaticTask final
{
public:
    StaticTask() = default;
    StaticTask(const StaticTask&) = delete;
    StaticTask& operator=(const StaticTask&) = delete;
    StaticTask(StaticTask&&) = delete;
    StaticTask& operator=(StaticTask&&) = delete;

    bool create(TaskFunction_t function,
                const char* const name,
                void* const argument,
                const UBaseType_t priority) noexcept
    {
        handle_ = xTaskCreateStatic(function,
                                    name,
                                    StackWords,
                                    argument,
                                    priority,
                                    stack_,
                                    &control_block_);
        return handle_ != nullptr;
    }

    TaskHandle_t native() const noexcept
    {
        return handle_;
    }

private:
    StaticTask_t control_block_{};
    StackType_t stack_[StackWords]{};
    TaskHandle_t handle_{nullptr};
};
}
