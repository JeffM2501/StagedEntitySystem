#pragma once

#include "ComponentSerialization.h"
namespace Components
{
inline void SerializeSpriteReference(const rapidjson::Value& json, BufferWriter& out)
{
	SeralizeAssetReference("Sheet", json, out);
	SerializeNumber<int32_t>("Frame", 0, json, out);
	SerializeNumber<float>("Rotation", 0, json, out);
}

inline void SerializeTransformComponent(const rapidjson::Value& json, BufferWriter& out)
{
	SerializeNumberArray<float>("Position", {0,0}, json, out);
	SerializeNumberArray<float>("Velocity", {0,0}, json, out);
}

inline void SerializePlayerComponent(const rapidjson::Value& json, BufferWriter& out)
{
	SerializeNumber<float>("Size", 10, json, out);
	SerializeNumber<float>("Health", 100, json, out);
	SerializeNumber<float>("PlayerSeed", 100, json, out);
	SerializeNumber<float>("ReloadTime", 0.25, json, out);
	SeralizeAssetReference("BulletPrefab", json, out);
	SerializeNumber<float>("BulletSpreadDelta", 5.0, json, out);
	SerializeNumber<float>("BulletSpeedMultiplier", 2.0, json, out);
	SerializeNumber<float>("BulletSpeedVariance", 1.5f, json, out);
	if (json.HasMember("Sprite"))
		SerializeSpriteReference(json["Sprite"], out);
}

inline void SerializeBulletComponent(const rapidjson::Value& json, BufferWriter& out)
{
	SerializeNumber<float>("Size", 4, json, out);
	SerializeNumber<float>("Damage", 10, json, out);
	SerializeNumber<float>("Lifetime", 1, json, out);
	SerializeColor("Tint", {255,255,0,255}, json, out);
	if (json.HasMember("Sprite"))
		SerializeSpriteReference(json["Sprite"], out);
}

inline void SerializeNPCComponent(const rapidjson::Value& json, BufferWriter& out)
{
	SerializeNumber<float>("Size", 20, json, out);
	SerializeColor("Tint", {0, 0, 255, 255}, json, out);
	if (json.HasMember("Sprite"))
		SerializeSpriteReference(json["Sprite"], out);
}

inline void SerializePlayerSpawnComponent(const rapidjson::Value& json, BufferWriter& out)
{
	SeralizeAssetReference("PlayerPrefab", json, out);
}

inline void SerializeNPCSpawnComponent(const rapidjson::Value& json, BufferWriter& out)
{
	SerializeNumber<float>("MinInterval", 1, json, out);
	SerializeNumber<float>("MaxInterval", 3, json, out);
	SerializeNumber<float>("MinVelocity", 20, json, out);
	SerializeNumber<float>("MaxVelocity", 200l, json, out);
	SerializeNumber<int32_t>("MaxSpawnCount", 100, json, out);
	SeralizeAssetReference("NPCPrefab", json, out);
}

inline void RegisterCompoenents()
{
	ComponentSerialization::Register("TransformComponent",  SerializeTransformComponent);
	ComponentSerialization::Register("PlayerComponent",  SerializePlayerComponent);
	ComponentSerialization::Register("BulletComponent",  SerializeBulletComponent);
	ComponentSerialization::Register("NPCComponent",  SerializeNPCComponent);
	ComponentSerialization::Register("PlayerSpawnComponent",  SerializePlayerSpawnComponent);
	ComponentSerialization::Register("NPCSpawnComponent",  SerializeNPCSpawnComponent);
}
} //Components
