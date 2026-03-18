#pragma once

#include "EntitySystem.h"
#include "components/data/ComponentData.h"

struct TransformComponent : public EntitySystem::EntityComponent
{
    DECLARE_SIMPLE_COMPONENT(TransformComponent);

    bool OnDataRead(BufferReader& buffer) override
    {
        Data.Read(buffer);

        TraceLog(LOG_INFO, "Loaded Transform for entity %zu", EntityID);

        return true;
    }

    Components::TransformComponentData Data;
};

