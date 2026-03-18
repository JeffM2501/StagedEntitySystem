#pragma once

#include "EntitySystem.h"

#include "components/data/ComponentData.h"

struct PlayerSpawnComponent : public EntitySystem::EntityComponent
{
    DECLARE_SIMPLE_COMPONENT(PlayerSpawnComponent);

    double LastUpdateTime = 0;

    Components::PlayerSpawnComponentData Data;

    void OnAwake() override;
    bool OnDataRead(BufferReader& buffer) override;
};
