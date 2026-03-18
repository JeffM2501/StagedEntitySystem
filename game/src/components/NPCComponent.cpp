#include "components/NPCComponent.h"
#include "components/TransformComponent.h"

#include "TimeUtils.h"
#include "TaskManager.h"
#include "GameInfo.h"

bool MoveEntity(TransformComponent& entity, float size, Vector2 delta, const BoundingBox2D& bounds)
{
    bool hit = false;

    Vector2 newPos = entity.Data.Position + delta;

    if (newPos.x > bounds.Max.x - size)
    {
        newPos.x = bounds.Max.x - size;
        entity.Data.Velocity.x *= -1;
        hit = true;
    }
    else if (newPos.x < bounds.Min.x + size)
    {
        newPos.x = bounds.Min.x + size;
        entity.Data.Velocity.x *= -1;
        hit = true;
    }

    if (newPos.y > bounds.Max.y - size)
    {
        newPos.y = bounds.Max.y - size;
        entity.Data.Velocity.y *= -1;
        hit = true;
    }
    else if (newPos.y < bounds.Min.y + size)
    {
        newPos.y = bounds.Min.y + size;
        entity.Data.Velocity.y *= -1;
        hit = true;
    }

    entity.Data.Position = newPos;
    return hit;
}

void NPCComponent::OnAwake()
{
}

void NPCComponent::Update()
{
    auto transform = GetEntityComponent<TransformComponent>();
    if (transform)
    {
        float realSize = Data.Sprite.SpriteRef->GetFrameRect(Data.Sprite.CurrentFrame).width * Data.Sprite.Scale;
        float delta = TaskManager::GetFixedDeltaTime();
        MoveEntity(*transform, realSize*0.5f, transform->Data.Velocity * delta, WorldBounds.load());
        LastUpdateTime = GetFrameStartTime();
    }
}

bool NPCComponent::OnDataRead(BufferReader& buffer)
{
    Data.Read(buffer);

    TraceLog(LOG_INFO, "Loaded NPCComponent for entity %zu", EntityID);

    return true;
}