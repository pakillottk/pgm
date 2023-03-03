#pragma once

#include "../Components/MetadataComponent.h"
#include "../Scene/Scene.h"

#include "../../Core/Assert/Assert.h"
#include <entt/entt.hpp>

namespace PGM
{

using entity_id = entt::entity;

class EntityRef
{
  public:
    ~EntityRef() = default;

    EntityRef(const EntityRef &) = default;
    EntityRef &operator=(const EntityRef &) = default;

    EntityRef(EntityRef &&) = default;
    EntityRef &operator=(EntityRef &&) = default;

    operator bool() const
    {
        return m_Handle != entt::null;
    }

    constexpr entity_id id() const
    {
        return m_Handle;
    }

    template <typename Component> inline bool hasComponent()
    {
        PGM_ASSERT(m_Handle != entt::null, "Null entity ref");
        return m_Scene->m_Registry.any_of<Component>(m_Handle);
    }

    template <typename Component, typename... Args> Component &emplaceComponent(Args &&...args)
    {
        PGM_ASSERT(m_Handle != entt::null, "Null entity ref");
        PGM_ASSERT(!hasComponent<Component>(), "Duplicated component");

        Component &component = m_Scene->m_Registry.emplace<Component>(m_Handle, std::forward<Args>(args)...);

        auto &meta = getComponent<Components::MetadataComponent>();
        meta.metadata.emplace_back(component);

        return component;
    }

    template <typename Component> Component &getComponent()
    {
        PGM_ASSERT(hasComponent<Component>(), "Component not found...");
        return m_Scene->m_Registry.get<Component>(m_Handle);
    }

  private:
    constexpr EntityRef(entt::entity id, Scene *scene) : m_Handle{id}, m_Scene{scene}
    {
    }

    entt::entity m_Handle = {entt::null};
    Scene *m_Scene;

    friend class Scene;
};
} // namespace PGM