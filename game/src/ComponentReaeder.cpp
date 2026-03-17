#include "ComponentReader.h"

#include "components/TransformComponent.h"
#include "components/PlayerComponent.h"
#include "components/NPCComponent.h"
#include "components/BulletComponent.h"
#include "components/PlayerSpawnComponent.h"
#include "components/NPCSpawnComponent.h"

void ComponentReader::OnComponentData(EntitySystem::EntityComponent* component, size_t componentId, BufferReader& buffer)
{
    if (component != nullptr)
        component->OnDataRead(buffer);
}
