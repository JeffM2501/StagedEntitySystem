#pragma once

#include "EntitySystem.h"
#include "components/data/ComponentData.h"

struct NPCSpawnComponent : public EntitySystem::EntityComponent
{
    DECLARE_SIMPLE_COMPONENT(NPCSpawnComponent);

    Components::NPCSpawnComponentData Data;

    double LastUpdateTime = 0;
    float NextSpawnInterval = 0;

    void OnAwake() override;

    void Update();

    bool OnDataRead(BufferReader& buffer) override;
};
