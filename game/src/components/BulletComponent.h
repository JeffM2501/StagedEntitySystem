#pragma once
#pragma once

#include "EntitySystem.h"
#include "components/data/ComponentData.h"

struct BulletComponent : public EntitySystem::EntityComponent
{
    DECLARE_SIMPLE_COMPONENT(BulletComponent);

    Components::BulletComponentData Data;

    float SpinDir = 1.0f;
    double LastUpdateTime = 0;

    void Update();

    void OnAwake() override;
    bool OnDataRead(BufferReader& buffer) override;
};
