#pragma once

#include "ApplicationSystem.h"

#include <PGM/Core/Ref/Ref.h>

#include <vector>

namespace PGM
{

class Application;

class ApplicationSystemStack final
{
  public:
    inline ApplicationSystemStack(const Application &app) : m_App(app)
    {
    }

    inline ~ApplicationSystemStack()
    {
        for (auto &system : m_Stack)
        {
            system->onDeactivate(m_App);
        }
    }

    template <typename SystemType, typename... Args> inline void push(Args &&...args)
    {
        auto system = make_shared_ref<SystemType>(std::forward<Args>(args)...);
        system->onActivate(m_App);
        m_Stack.push_back(system);
    }

    inline auto begin() noexcept
    {
        return m_Stack.rbegin();
    }

    inline auto end() noexcept
    {
        return m_Stack.rend();
    }

  private:
    const Application &m_App;
    std::vector<SharedRef<ApplicationSystem>> m_Stack;
};

} // namespace PGM