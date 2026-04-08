#include "components/SpriteComponent.h"
#include "components/TransformComponent.h"

#include "TimeUtils.h"
#include "TaskManager.h"
#include "GameInfo.h"

void SpriteComponent::OnAwake()
{
}

void SpriteComponent::Update()
{
}

bool SpriteComponent::OnDataRead(BufferReader& buffer)
{
    Data.Read(buffer);

    TraceLog(LOG_INFO, "Loaded SpriteComponent for entity %zu", EntityID);

    return true;
}