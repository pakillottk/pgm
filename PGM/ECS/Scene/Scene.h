#pragma once

#include <entt/entt.hpp>

#include <functional>
#include <string>

namespace PGM::ECS
{

class EntityRef;

class Scene
{
  public:
    EntityRef createEntity(std::string name);
    void removeEntity(const EntityRef &ent);

    void for_each(std::function<void(EntityRef)> callback);

  private:
    entt::registry m_Registry;

    friend EntityRef;
};

} // namespace PGM::ECS