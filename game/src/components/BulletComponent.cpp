#include "components/BulletComponent.h"
#include "components/TransformComponent.h"

#include "TimeUtils.h"

#include "raylib.h"
#include "raymath.h"

void BulletComponent::Update()
{
    Data.Lifetime -= GetDeltaTime();
    if (Data.Lifetime < 0)
    {
        EntitySystem::RemoveEntity(EntityID);
        return;
    }

    auto transform = GetEntityComponent<TransformComponent>();
    if (transform)
    {
        transform->Data.Position += transform->Data.Velocity * GetDeltaTime();
    }

    Data.Sprite.Rotation += 1000 * GetDeltaTime() * SpinDir;
    Data.Sprite.Rotation = fmodf(Data.Sprite.Rotation, 360);
}

void BulletComponent::OnAwake()
{
    SpinDir = GetRandomValue(0, 1) == 0 ? -1.0f : 1.0f;
}

bool BulletComponent::OnDataRead(BufferReader& buffer)
{
    Data.Read(buffer);
    TraceLog(LOG_INFO, "Loaded BulletComponent for entity %zu", EntityID);
    return true;
}
