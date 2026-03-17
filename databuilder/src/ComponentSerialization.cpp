#include "ComponentSerialization.h"
#include "components.h"
#include "SerializationUtils.h"

#include <unordered_map>
#include <functional>

namespace ComponentSerialization
{ 
    std::unordered_map<std::string, std::function<void(const rapidjson::Value&, BufferWriter&)>> Serializers;
    
    void SetupSerializers()
    {
        Components::RegisterCompoenents();
    }

    void Register(const std::string& type, std::function<void(const rapidjson::Value&, BufferWriter&)> serializer)
    {
        Serializers[type] = serializer;
    }

    void Serialize(const std::string& type, const rapidjson::Value& j, BufferWriter& out)
    {
        auto itr = Serializers.find(type);
        if (itr != Serializers.end())
        {
            itr->second(j, out);
        }
    }
}