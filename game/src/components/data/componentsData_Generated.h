#pragma once
#include "BufferReader.h"
#include "SpriteManager.h"
#include "raymath.h"

namespace Components
{
struct TransformComponentData
{
	Vector2 Position = {0,0};
	Vector2 Velocity = {0,0};

	void Read(BufferReader& buffer)
	{
		Position = buffer.Read<Vector2>();
		Velocity = buffer.Read<Vector2>();
	}
};

struct PlayerComponentData
{
	float Size = 10;
	float Health = 100;
	float PlayerSeed = 100;
	float ReloadTime = 0.25;
	size_t BulletPrefab;
	float BulletSpreadDelta = 5.0;
	float BulletSpeedMultiplier = 2.0;
	float BulletSpeedVariance = 1.5f;
	SpriteManager::SpriteInstance Sprite;

	void Read(BufferReader& buffer)
	{
		Size = buffer.Read<float>();
		Health = buffer.Read<float>();
		PlayerSeed = buffer.Read<float>();
		ReloadTime = buffer.Read<float>();
		BulletPrefab = buffer.Read<size_t>();
		BulletSpreadDelta = buffer.Read<float>();
		BulletSpeedMultiplier = buffer.Read<float>();
		BulletSpeedVariance = buffer.Read<float>();
		Sprite = SpriteManager::LoadFromBuffer(buffer);
	}
};

struct BulletComponentData
{
	float Size = 4;
	float Damage = 10;
	float Lifetime = 1;
	Color Tint = {255,255,0,255};
	SpriteManager::SpriteInstance Sprite;

	void Read(BufferReader& buffer)
	{
		Size = buffer.Read<float>();
		Damage = buffer.Read<float>();
		Lifetime = buffer.Read<float>();
		Tint = buffer.Read<Color>();
		Sprite = SpriteManager::LoadFromBuffer(buffer);
	}
};

struct NPCComponentData
{
	float Size = 20;
	Color Tint = {0, 0, 255, 255};
	SpriteManager::SpriteInstance Sprite;

	void Read(BufferReader& buffer)
	{
		Size = buffer.Read<float>();
		Tint = buffer.Read<Color>();
		Sprite = SpriteManager::LoadFromBuffer(buffer);
	}
};

struct PlayerSpawnComponentData
{
	size_t PlayerPrefab;

	void Read(BufferReader& buffer)
	{
		PlayerPrefab = buffer.Read<size_t>();
	}
};

struct NPCSpawnComponentData
{
	float MinInterval = 1;
	float MaxInterval = 3;
	float MinVelocity = 20;
	float MaxVelocity = 200l;
	int32_t MaxSpawnCount = 100;
	size_t NPCPrefab;

	void Read(BufferReader& buffer)
	{
		MinInterval = buffer.Read<float>();
		MaxInterval = buffer.Read<float>();
		MinVelocity = buffer.Read<float>();
		MaxVelocity = buffer.Read<float>();
		MaxSpawnCount = buffer.Read<int32_t>();
		NPCPrefab = buffer.Read<size_t>();
	}
};

} //Components
