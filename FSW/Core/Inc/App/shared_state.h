#pragma once

#include "App/messages.h"
#include "App/static_mutex.h"

namespace adcs::app
{
class SharedAdcsState final
{
public:
    bool init() noexcept;
    void writeSnapshot(const AdcsSnapshot& snapshot) noexcept;
    AdcsSnapshot readSnapshot() noexcept;
    void writeFault(const FaultStatus& fault) noexcept;
    FaultStatus readFault() noexcept;

private:
    StaticMutex mutex_{};
    AdcsSnapshot snapshot_{};
    FaultStatus fault_{};
};

namespace detail
{
extern SharedAdcsState g_shared_state;
}

inline bool SharedAdcsState::init() noexcept
{
    snapshot_ = AdcsSnapshot{0U, AdcsMode::Boot,
                             {1.0F, 0.0F, 0.0F, 0.0F},
                             {0.0F, 0.0F, 0.0F},
                             {0.0F, 0.0F, 0.0F},
                             {0.0F, 0.0F, 0.0F},
                             false,
                             false};
    fault_ = FaultStatus{0U};
    return mutex_.init();
}

inline void SharedAdcsState::writeSnapshot(const AdcsSnapshot& snapshot) noexcept
{
    MutexGuard guard{mutex_};
    if (guard.locked())
    {
        snapshot_ = snapshot;
    }
}

inline AdcsSnapshot SharedAdcsState::readSnapshot() noexcept
{
    MutexGuard guard{mutex_};
    if (guard.locked())
    {
        return snapshot_;
    }
    return AdcsSnapshot{};
}

inline void SharedAdcsState::writeFault(const FaultStatus& fault) noexcept
{
    MutexGuard guard{mutex_};
    if (guard.locked())
    {
        fault_ = fault;
    }
}

inline FaultStatus SharedAdcsState::readFault() noexcept
{
    MutexGuard guard{mutex_};
    if (guard.locked())
    {
        return fault_;
    }
    return FaultStatus{0xFFFFFFFFUL};
}

inline SharedAdcsState& sharedState() noexcept
{
    return detail::g_shared_state;
}
}
