#pragma once

#include "EntitySystem.h"
#include "components/data/componentsData_Generated.h"

struct SpriteComponent : public EntitySystem::EntityComponent
{
    DECLARE_SIMPLE_COMPONENT(SpriteComponent);

    Components::SpriteComponentData Data;

    double LastUpdateTime = 0;
    void Update();
    void OnAwake() override;
    bool OnDataRead(BufferReader& buffer) override;
};
