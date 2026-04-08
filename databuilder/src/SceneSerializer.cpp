#include "SceneSerializer.h"
#include "ComponentSerialization.h"

#include "rapidjson/prettywriter.h"

#include <iostream>
#include <fstream>

static constexpr uint32_t SceneMagic = 0x53434E45; // "SCNE" in ASCII
static constexpr uint32_t SceneVersion = 1;

void SerializeEntities(BufferWriter& buffer, rapidjson::Document& scene, rapidjson::Value* overrides = nullptr);

extern std::string_view GetInputFolder();

bool LoadPrefab(rapidjson::Value& prefabRefence, rapidjson::Document& prefabDocument)
{
    std::string file;
    auto prefabItr = prefabRefence.FindMember("Prefab");
    if (prefabItr == prefabRefence.MemberEnd() || !prefabItr->value.IsString())
    {
        std::cerr << "Prefab reference missing 'Prefab' field or it is not a string." << std::endl;
        return false;
    }
    file = prefabItr->value.GetString();

    file = std::string(GetInputFolder()) + "/" + file;

    std::ifstream in(file);
    std::string jsonStr((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    if (prefabDocument.Parse(jsonStr.c_str()).HasParseError())
    {
        std::cerr << "Failed to parse JSON file: " << file << std::endl;
        return false;
    }

    return true;
}

void SerializePrefab(rapidjson::Value& prefabRefence, BufferWriter& buffer)
{
    rapidjson::Document prefab;
    if (!LoadPrefab(prefabRefence, prefab))
        return;

    auto overrides = prefabRefence.FindMember("Overrides");
    
    SerializeEntities(buffer, prefab, &overrides->value);
}

void SerializeEntities(BufferWriter& buffer, rapidjson::Document& scene, rapidjson::Value* overrides)
{
    auto entityList = scene.FindMember("Entities");
    if (entityList == scene.MemberEnd() || !entityList->value.IsArray())
    {
        return;
    }

    for (auto& entity : entityList->value.GetArray())
    {
        bool isPrefab = false;
        auto type = entity.FindMember("$type");
        if (type != entity.MemberEnd() && type->value.IsString() && std::string(type->value.GetString()) == "prefab")
            isPrefab = true;

        if (isPrefab)
        {
            SerializePrefab(entity, buffer);
            continue;
        }
        int64_t entityId = entity["ID"].GetInt64();
        auto nameItr = entity.FindMember("Name");
        std::string name = (nameItr != entity.MemberEnd() && nameItr->value.IsString()) ? nameItr->value.GetString() : std::to_string(entityId);

        rapidjson::Value* entityOverrides = nullptr;
        if (overrides)
        {
            auto itr = overrides->FindMember(name.c_str());
            if (itr != overrides->MemberEnd() && itr->value.IsObject())
                entityOverrides = &itr->value;
        }

        auto componentList = entity.FindMember("Components");

        if (componentList->value.IsObject())
        {
            uint32_t componentCount = 0;
            for (auto& m : componentList->value.GetObject())
                ++componentCount;

            buffer.Write(entityId);
            buffer.Write(componentCount);

            for (auto& m : componentList->value.GetObject())
            {
                const char* name = m.name.GetString();
                rapidjson::Value& compValue = m.value;

                // Use member name as type if the component doesn't include a "Type" field
                std::string type;
                auto typeIt = compValue.FindMember("Type");
                if (typeIt != compValue.MemberEnd() && typeIt->value.IsString())
                    type = typeIt->value.GetString();
                else
                    type = name;

                if (entityOverrides) // this entity has overrides
                {
                    auto compOverride = entityOverrides->FindMember(name);
                    if (compOverride != entityOverrides->MemberEnd() && compOverride->value.IsObject())
                    {
                        // we have an override for this component
                        for (auto& fieldOverride : compOverride->value.GetObject())
                        {
                            std::string fieldName = fieldOverride.name.GetString();

                            // replace the field or add it
                            auto compField = compValue.FindMember(fieldName.c_str());
                            if (compField != compValue.MemberEnd())
                            {
                                compValue.RemoveMember(compField);
                            }
                 
                            rapidjson::Value newValue;
                            newValue.CopyFrom(fieldOverride.value, scene.GetAllocator());

                            rapidjson::Value nameValue;
                            nameValue.SetString(fieldName.c_str(), rapidjson::SizeType(fieldName.size()), scene.GetAllocator());

                            compValue.AddMember(std::move(nameValue), fieldOverride.value, scene.GetAllocator());

                            for (auto& sanity : compValue.GetObject())
                            {
                                auto sanityName = sanity.name.GetString();

                                if (sanity.value.IsObject())
                                {
                                    std::cout << "object " << sanityName << std::endl;
                                }
                                if (sanity.value.IsArray())
                                {
                                    std::cout << "array " << sanityName << std::endl;
                                }
                            }
                        }
                    }
                }

                uint64_t componentId = Hashes::CRC64Str(type);

                buffer.Write(componentId);

                BufferWriter compData;
                ComponentSerialization::Serialize(type, compValue, compData);

                buffer.WriteBufferr(compData);
            }
// 
//             // Pretty format (with indentation)
//             rapidjson::StringBuffer buffer;
//             rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
//             scene.Accept(writer);
//             std::string json = buffer.GetString();
// 
//             int i = 0;
        }
    }
}

void SerializeScene(BufferWriter& buffer, rapidjson::Document& scene)
{
    // header
    buffer.Write(SceneMagic);
    buffer.Write(SceneVersion);

    SerializeEntities(buffer, scene);
}
