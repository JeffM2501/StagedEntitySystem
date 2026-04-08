#pragma once

#include "EntitySystem.h"
#include "components/data/componentsData_Generated.h"

struct PlayerComponent : public EntitySystem::EntityComponent
{
    DECLARE_SIMPLE_COMPONENT(PlayerComponent);

    Vector2 Input = Vector2Zeros;

    bool ShootThisFrame = false;
    float LastShotTime = 0;

    Components::PlayerComponentData Data;

    void Update();

    void OnAwake() override;

    bool OnDataRead(BufferReader& buffer) override;
};