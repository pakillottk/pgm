#pragma once

#include <PGM/Core/Ref/Ref.h>
#include <vector>

namespace PGM::ECS::Components
{

class ComponentMeta
{
  public:
    template <typename Component> constexpr ComponentMeta([[maybe_unused]] const Component &cmp)
    {
    }

    // TODO: Copy, render editor...

  private:
    struct ComponentOpsConcept
    {
        virtual ~ComponentOpsConcept() = default;

        // TODO: Copy, render editor...
    };

    template <typename Component> struct ComponentOpsModel : ComponentOpsConcept
    {
        // TODO: Copy, render editor...
    };

    Ref<ComponentOpsConcept> m_Ops;
};

struct MetadataComponent
{
    std::vector<ComponentMeta> metadata;

    MetadataComponent() = default;
    ~MetadataComponent() = default;

    MetadataComponent(MetadataComponent &&) = default;
    MetadataComponent &operator=(MetadataComponent &&) = default;

    MetadataComponent(const MetadataComponent &) = delete;
    MetadataComponent &operator=(const MetadataComponent &) = delete;
};

} // namespace PGM::ECS::Components