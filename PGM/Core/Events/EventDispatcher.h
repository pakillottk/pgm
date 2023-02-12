#pragma once

#include "../Assert/Assert.h"
#include "../Logging/Logger.h"
#include "../Ref/Ref.h"

#include <algorithm>
#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace PGM::Events
{

using event_id = size_t;
using listener_id = size_t;

class EventQueue;

struct EventListener final
{
    inline ~EventListener()
    {
        if (m_UnsuscribeCallback)
        {
            m_UnsuscribeCallback();
        }
    }

    EventListener(EventListener &&) noexcept = default;
    EventListener &operator=(EventListener &&) = default;

    EventListener(const EventListener &) = delete;
    EventListener &operator=(const EventListener &) = delete;

  private:
    std::function<void()> m_UnsuscribeCallback;

    inline EventListener(std::function<void()> unsuscribeCallback) : m_UnsuscribeCallback{std::move(unsuscribeCallback)}
    {
    }

    friend EventQueue;
};

using ListenersMap = std::unordered_map<listener_id, std::function<void(const void *)>>;
using EventDispatchTable = std::unordered_map<event_id, ListenersMap>;

class EventQueue
{
  public:
    template <typename EventType>
    inline EventListener suscribe(const std::function<void(const EventType &event)> &callback)
    {
        std::scoped_lock<std::mutex> lk{m_Lock};

        const auto id = genId();
        auto callbackPtr = [callback](const void *event_ptr) {
            callback(*reinterpret_cast<const EventType *>(event_ptr));
        };

        auto queue = m_Listeners.find(typeid(EventType).hash_code());
        if (queue == m_Listeners.end())
        {
            m_Listeners[typeid(EventType).hash_code()] = ListenersMap{{id, callbackPtr}};
        }
        else
        {
            queue->second[id] = callbackPtr;
        }

        return EventListener{[this, id]() { this->unsuscribe<EventType>(id); }};
    }

    template <typename EventType> inline void unsuscribe(listener_id id)
    {
        std::scoped_lock<std::mutex> lk{m_Lock};

        // Find queue of event
        auto queueIt = m_Listeners.find(typeid(EventType).hash_code());
        PGM_ASSERT(queueIt != m_Listeners.end(), "Queue not found");
        ListenersMap &queue = queueIt->second;

        // Remove listener with id
        queue.erase(id);
    }

  protected:
    template <typename EventType> inline void dispatch(const EventType &event) const
    {
        std::unique_lock<std::mutex> lk{m_Lock, std::defer_lock};
        // FIXME(pgm) This is to prevent a nasty exception if an event is dispatched while handling another event
        // dispatch
        if (++m_DispatchLevel == 1)
        {
            lk.lock();
        }

        // Logging::log_debug("(EventDispatcher) Dispatched event: {}", typeid(EventType).name());

        // Find queue of event
        auto queueIt = m_Listeners.find(typeid(EventType).hash_code());
        if (queueIt != m_Listeners.cend())
        {

            const auto &queue = queueIt->second;
            std::for_each(queue.cbegin(), queue.cend(),
                          [&event](const auto &listener) { listener.second(reinterpret_cast<const void *>(&event)); });
        }
        // else: no suscribers to this event

        --m_DispatchLevel;
    }

  private:
    static std::atomic<listener_id> g_LastListenerId;
    static inline listener_id genId()
    {
        return ++g_LastListenerId;
    }

    mutable std::mutex m_Lock;
    mutable std::atomic<size_t> m_DispatchLevel{0};
    EventDispatchTable m_Listeners;
};

class EventDispatcher final : public EventQueue
{
  public:
    template <typename EventType> inline void dispatch(const EventType &event) const
    {
        EventQueue::dispatch<EventType>(event);
    }

    template <typename EventType, typename... Args> inline void emplace_dispatch(Args &&...args)
    {
        EventQueue::dispatch<EventType>(EventType{std::forward<Args>(args)...});
    }
};

} // namespace PGM::Events