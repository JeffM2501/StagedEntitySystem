#include "PlayerSpawnComponent.h"
#include "components/TransformComponent.h"
#include "GameInfo.h"

void PlayerSpawnComponent::OnAwake()
{
    auto transform = GetEntityComponent<TransformComponent>();
    if (transform)
    {
        Vector2 pos = transform->Data.Position;

        PrefabReader.ReadEntitiesFromResource(Data.PlayerPrefab, [pos](std::span<size_t> entities)
            {
                EntitySystem::GetEntityComponent<TransformComponent>(entities[0])->Data.Position = pos;
            });
    }
}

bool PlayerSpawnComponent::OnDataRead(BufferReader& buffer)
{
    Data.Read(buffer);

    TraceLog(LOG_INFO, "Loaded PlayerSpawnComponent for entity %zu", EntityID);
    return true;
}

