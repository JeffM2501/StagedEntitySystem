#pragma once

#include "EntitySystem.h"
#include "components/data/ComponentData.h"

struct NPCComponent : public EntitySystem::EntityComponent
{
    DECLARE_SIMPLE_COMPONENT(NPCComponent);

    Components::NPCComponentData Data;

    double LastUpdateTime = 0;
    void Update();
    void OnAwake() override;
    bool OnDataRead(BufferReader& buffer) override;
};
