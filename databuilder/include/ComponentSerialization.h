#pragma once

#include <string>
#include <functional>
#include <rapidjson/document.h>

#include "SerializationUtils.h"

// Serialization interface for components
namespace ComponentSerialization
{
    void SetupSerializers();

    // General dispatcher for serialization by component type name
    void Serialize(const std::string& type, const rapidjson::Value& j, BufferWriter& out);

    void Register(const std::string& type, std::function<void(const rapidjson::Value&, BufferWriter&)> serializer);
}
