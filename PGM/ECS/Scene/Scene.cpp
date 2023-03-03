#include "Scene.h"

#include "../Components/Components.h"
#include "../Entity/EntityRef.h"

namespace PGM
{

EntityRef Scene::createEntity(std::string name)
{
    EntityRef ent{m_Registry.create(), this};

    ent.emplaceComponent<Components::MetadataComponent>();
    ent.emplaceComponent<Components::TagComponent>(name);
    ent.emplaceComponent<Components::TransformComponent>();

    return ent;
}

void Scene::removeEntity(const EntityRef &ent)
{
    PGM_ASSERT(ent, "Null entity ref");
    m_Registry.destroy(ent.id());
}

void Scene::for_each(std::function<void(EntityRef)> callback)
{
    m_Registry.each([this, &callback](auto handle) { callback(EntityRef{handle, this}); });
}

} // namespace PGM