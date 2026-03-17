#pragma once

#include "raylib.h"

#include "EntitySystem.h"
#include "TransformComponent.h"
#include "SpriteManager.h"

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
