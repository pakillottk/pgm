#pragma once

#include <entt/entt.hpp>

#include <functional>
#include <string>

namespace PGM
{

class EntityRef;

class Scene
{
  public:
    EntityRef createEntity(std::string name);
    void removeEntity(const EntityRef &ent);

    void for_each(std::function<void(EntityRef)> callback);

    template <typename Component, typename... Components> inline auto view()
    {
        return m_Registry.view<Component, Components...>();
    }

  private:
    entt::registry m_Registry;

    friend EntityRef;
};

} // namespace PGM