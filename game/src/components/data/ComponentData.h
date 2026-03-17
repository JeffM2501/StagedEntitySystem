#pragma once

#include "BufferReader.h"
#include "SpriteManager.h"

namespace Components
{
    struct BulletComponentData
    {
        float Size = 4;
        float Damage = 10;
        float Lifetime = 1;
        Color Tint = { 255,255,0,255 };
        SpriteManager::SpriteInstance Sprite;


        void Read(BufferReader& buffer)
        {
            Size = buffer.Read<float>();
            Damage = buffer.Read<float>();
            Lifetime = buffer.Read<float>();
            Tint = buffer.ReadColor();

            Sprite = SpriteManager::LoadFromBuffer(buffer);
        }
    };

    struct NPCComponentData
    {
        float Size = 20;
        Color Tint = BLUE;
        SpriteManager::SpriteInstance Sprite;

        void Read(BufferReader& buffer)
        {
            Size = buffer.Read<float>();
            Tint = buffer.ReadColor();

            Sprite = SpriteManager::LoadFromBuffer(buffer);
        }
    };
}