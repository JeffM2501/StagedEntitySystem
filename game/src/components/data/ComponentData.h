#pragma once

#include "BufferReader.h"
#include "SpriteManager.h"
#include "raymath.h"

namespace Components_old
{
    struct TransformComponentData
    {
        Vector2 Position = Vector2Zeros;
        Vector2 Velocity = Vector2Zeros;

        void Read(BufferReader& buffer)
        {
            Position.x = buffer.Read<float>();
            Position.y = buffer.Read<float>();
            Velocity.x = buffer.Read<float>();
            Velocity.y = buffer.Read<float>();
        }
    };

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

    struct NPCSpawnComponentData
    {
        float MinInterval = 1;
        float MaxInterval = 3;
        float MinVelocity = 20;
        float MaxVelocity = 200;
        uint32_t MaxSpawnCount = 100;

        size_t NPCPrefab = 0;

        void Read(BufferReader& buffer)
        {
            MinInterval = buffer.Read<float>();
            MaxInterval = buffer.Read<float>();
            MinVelocity = buffer.Read<float>();
            MaxVelocity = buffer.Read<float>();
            MaxSpawnCount = buffer.Read<uint32_t>();
            NPCPrefab = buffer.Read<size_t>();
        }
    };
    
    struct PlayerComponentData
    {
        float Size = 10;
        float Health = 100;
        float PlayerSpeed = 100;
        float ReloadTime = 0.25f;
        size_t BulletPrefab = 0;
        float ShotSpread = 5;
        float ShotSpeedMultiplyer = 2;
        float ShotSpeedVariance = 1.5f;
        SpriteManager::SpriteInstance Sprite;

        void Read(BufferReader& buffer)
        {
            Size = buffer.Read<float>();
            Health = buffer.Read<float>();
            PlayerSpeed = buffer.Read<float>();
            ReloadTime = buffer.Read<float>();
            BulletPrefab = buffer.Read<size_t>();
            ShotSpread = buffer.Read<float>();
            ShotSpeedMultiplyer = buffer.Read<float>();
            ShotSpeedVariance = buffer.Read<float>();

            Sprite = SpriteManager::LoadFromBuffer(buffer);
        }
    };

    struct PlayerSpawnComponentData
    {
        size_t PlayerPrefab = 0;

        void Read(BufferReader& buffer)
        {
            PlayerPrefab = buffer.Read<size_t>();
        }
    };
}