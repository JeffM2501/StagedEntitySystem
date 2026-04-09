#pragma once

#include <vector>
#include <string_view>
#include <span>
#include <cstdint>
#include <rapidjson/document.h>

#include "CRC64.h"

struct Vector2
{
    float x;
    float y;
};

struct Vector3
{
    float x;
    float y;
    float z;
};

struct Rectangle
{
    float x;
    float y;
    float width;
    float height;
};

struct Color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

class BufferWriter
{
public:
    std::vector<uint8_t> Buffer;

    template<typename T>
    inline void Write(const T& value)
    {
        Buffer.insert(Buffer.end(),
            reinterpret_cast<const uint8_t*>(&value),
            reinterpret_cast<const uint8_t*>(&value) + sizeof(T));
    }

    template<>
    inline void Write(const bool& value)
    {
        Write<uint8_t>(value ? (uint8_t)1 : uint8_t(0));
    }

    template<>
    inline void Write(const Vector2& value)
    {
        Write(value.x);
        Write(value.y);
    }

    template<>
    inline void Write(const Vector3& value)
    {
        Write(value.x);
        Write(value.y);
        Write(value.z);
    }

    template<>
    inline void Write(const Rectangle& value)
    {
        Write(value.x);
        Write(value.y);
        Write(value.width);
        Write(value.height);
    }

    template<typename T>
    inline void WriteArray(const std::vector<T>& value)
    {
        Write<size_t>(value.size());

        for (auto c : value)
        {
            Write<T>(c);
        }
    }

    inline void WriteBufferr(BufferWriter& buffer)
    {
        uint32_t size = uint32_t(buffer.Buffer.size());
        Write(size);
        Buffer.insert(Buffer.end(), buffer.Buffer.begin(), buffer.Buffer.end());
    }
};

inline bool ReadColor(uint8_t color[4], const rapidjson::Value& colorValue)
{
    if (colorValue.IsArray())
    {
        const auto& colorArray = colorValue;
        for (rapidjson::SizeType i = 0; i < colorArray.Size() && i < 4; ++i)
        {
            if (colorArray[i].IsUint())
                color[i] = static_cast<uint8_t>(colorArray[i].GetUint());
        }
        return true;
    }
    return false;
}

inline bool ReadValueBool(std::string_view name, bool& out, const rapidjson::Value& value)
{
    auto it = value.FindMember(name.data());
    if (it != value.MemberEnd() && it->value.IsBool())
    {
        out = it->value.GetBool();
        return true;
    }
    return false;
}

template<typename T>
inline bool ReadValueNumber(std::string_view name, T& out, const rapidjson::Value& value)
{
    auto it = value.FindMember(name.data());
    if (it != value.MemberEnd() && it->value.IsNumber())
    {
        out = static_cast<T>(it->value.Get<T>());
        return true;
    }
    return false;
}

template<>
inline bool ReadValueNumber(std::string_view name, uint8_t& out, const rapidjson::Value& value)
{
    auto it = value.FindMember(name.data());
    if (it != value.MemberEnd() && it->value.IsNumber())
    {
        out = static_cast<uint8_t>(it->value.GetUint());
        return true;
    }
    return false;
}

template<>
inline bool ReadValueNumber(std::string_view name, Vector2& out, const rapidjson::Value& value)
{
    auto it = value.FindMember(name.data());
    if (it != value.MemberEnd() && it->value.IsObject())
    {
        if (it->value.HasMember("x") && it->value["x"].IsNumber())
            out.x = it->value["x"].GetFloat();

        if (it->value.HasMember("y") && it->value["y"].IsNumber())
            out.y = it->value["y"].GetFloat();
        return true;
    }
    return false;
}

template<>
inline bool ReadValueNumber(std::string_view name, Vector3& out, const rapidjson::Value& value)
{
    auto it = value.FindMember(name.data());
    if (it != value.MemberEnd() && it->value.IsObject())
    {
        if (it->value.HasMember("x") && it->value["x"].IsNumber())
            out.x = it->value["x"].GetFloat();

        if (it->value.HasMember("y") && it->value["y"].IsNumber())
            out.y = it->value["y"].GetFloat();

        if (it->value.HasMember("z") && it->value["z"].IsNumber())
            out.z = it->value["z"].GetFloat();

        return true;
    }
    return false;
}

template<>
inline bool ReadValueNumber(std::string_view name,Rectangle& out, const rapidjson::Value& value)
{
    auto it = value.FindMember(name.data());
    if (it != value.MemberEnd() && it->value.IsObject())
    {
        if (it->value.HasMember("x") && it->value["x"].IsNumber())
            out.x = it->value["x"].GetFloat();

        if (it->value.HasMember("y") && it->value["y"].IsNumber())
            out.y = it->value["y"].GetFloat();

        if (it->value.HasMember("w") && it->value["w"].IsNumber())
            out.width = it->value["w"].GetFloat();

        if (it->value.HasMember("h") && it->value["h"].IsNumber())
            out.height = it->value["h"].GetFloat();

        return true;
    }
    return false;
}

template<typename T>
inline bool ReadValueNumberArray(std::string_view name, std::span<T> out, const rapidjson::Value& value)
{
    auto it = value.FindMember(name.data());
    if (it != value.MemberEnd() && it->value.IsArray())
    {
        const auto& valueArray = it->value;
        for (rapidjson::SizeType i = 0; i < valueArray.Size() && i < out.size(); ++i)
        {
            if (valueArray[i].IsNumber())
                out[i] = valueArray[i].Get<T>();
        }
        return true;
    }
    return false;
}

template<>
inline bool ReadValueNumberArray(std::string_view name, std::span<Vector2> out, const rapidjson::Value& value)
{
    auto it = value.FindMember(name.data());
    if (it != value.MemberEnd() && it->value.IsArray())
    {
        const auto& valueArray = it->value;
        for (rapidjson::SizeType i = 0; i < valueArray.Size() && i < out.size(); ++i)
        {
            if (valueArray[i].IsObject())
            {
                if (valueArray[i].HasMember("x") && valueArray[i]["x"].IsNumber())
                    out[i].x = it->value["x"].GetFloat();

                if (valueArray[i].HasMember("y") && valueArray[i]["y"].IsNumber())
                    out[i].y = it->value["y"].GetFloat();
            }
        }
        return true;
    }
    return false;
}

template<>
inline bool ReadValueNumberArray(std::string_view name, std::span<Vector3> out, const rapidjson::Value& value)
{
    auto it = value.FindMember(name.data());
    if (it != value.MemberEnd() && it->value.IsArray())
    {
        const auto& valueArray = it->value;
        for (rapidjson::SizeType i = 0; i < valueArray.Size() && i < out.size(); ++i)
        {
            if (valueArray[i].IsObject())
            {
                if (valueArray[i].HasMember("x") && valueArray[i]["x"].IsNumber())
                    out[i].x = it->value["x"].GetFloat();

                if (valueArray[i].HasMember("y") && valueArray[i]["y"].IsNumber())
                    out[i].y = it->value["y"].GetFloat();

                if (valueArray[i].HasMember("z") && valueArray[i]["z"].IsNumber())
                    out[i].z = it->value["z"].GetFloat();
            }
        }
        return true;
    }
    return false;
}

template<>
inline bool ReadValueNumberArray(std::string_view name, std::span<Rectangle> out, const rapidjson::Value& value)
{
    auto it = value.FindMember(name.data());
    if (it != value.MemberEnd() && it->value.IsArray())
    {
        const auto& valueArray = it->value;
        for (rapidjson::SizeType i = 0; i < valueArray.Size() && i < out.size(); ++i)
        {
            if (valueArray[i].IsObject())
            {
                if (valueArray[i].HasMember("x") && valueArray[i]["x"].IsNumber())
                    out[i].x = it->value["x"].GetFloat();

                if (valueArray[i].HasMember("y") && valueArray[i]["y"].IsNumber())
                    out[i].y = it->value["y"].GetFloat();

                if (valueArray[i].HasMember("w") && valueArray[i]["w"].IsNumber())
                    out[i].width = it->value["w"].GetFloat();

                if (valueArray[i].HasMember("h") && valueArray[i]["h"].IsNumber())
                    out[i].height = it->value["h"].GetFloat();
            }
        }
        return true;
    }
    return false;
}

inline void SerializeBool(std::string_view name, bool defaultValue, const rapidjson::Value& value, BufferWriter& out)
{
    bool binValue = defaultValue;
    ReadValueBool(name, binValue, value);
    out.Write(binValue);
}

template<typename T>
inline void SerializeNumber(std::string_view name, T defaultValue, const rapidjson::Value& value, BufferWriter& out)
{
    T binValue = defaultValue;
    ReadValueNumber(name, binValue, value);
    out.Write(binValue);
}

template<>
inline void SerializeNumber(std::string_view name, Vector2 defaultValue, const rapidjson::Value& value, BufferWriter& out)
{
    for (const auto& child : value.GetObject())
    {
        auto name = child.name.GetString();
        if (child.name == name)
        {
            if (child.value.IsArray())
            {
              
            }
        }
    }
    Vector2 binValue = defaultValue;
    const auto member = value.FindMember(name.data());
    if (member != value.MemberEnd() && member->value.IsArray())
    {
        binValue.x = member->value.GetArray()[0].GetFloat();
        binValue.y = member->value.GetArray()[1].GetFloat();
    }
    out.Write(binValue);
}

template<>
inline void SerializeNumber(std::string_view name, Vector3 defaultValue, const rapidjson::Value& value, BufferWriter& out)
{
    Vector3 binValue = defaultValue;
    const auto member = value.FindMember(name.data());
    if (member != value.MemberEnd() && member->value.IsArray())
    {
        binValue.x = member->value.GetArray()[0].GetFloat();
        binValue.y = member->value.GetArray()[1].GetFloat();
        binValue.z = member->value.GetArray()[2].GetFloat();
    }
    out.Write(binValue);
}

template<>
inline void SerializeNumber(std::string_view name, Rectangle defaultValue, const rapidjson::Value& value, BufferWriter& out)
{
    Rectangle binValue = defaultValue;
    const auto member = value.FindMember(name.data());
    if (member != value.MemberEnd() && member->value.IsArray())
    {
        binValue.x = member->value.GetArray()[0].GetFloat();
        binValue.y = member->value.GetArray()[1].GetFloat();
        binValue.width = member->value.GetArray()[2].GetFloat();
        binValue.height = member->value.GetArray()[3].GetFloat();
    }
    out.Write(binValue);
}

template<>
inline void SerializeNumber(std::string_view name, Color defaultValue, const rapidjson::Value& value, BufferWriter& out)
{
    Color binValue = defaultValue;
    const auto member = value.FindMember(name.data());
    if (member != value.MemberEnd() && member->value.IsArray())
    {
        binValue.r = uint8_t(member->value.GetArray()[0].GetInt());
        binValue.g = uint8_t(member->value.GetArray()[1].GetInt());
        binValue.b = uint8_t(member->value.GetArray()[2].GetInt());
        binValue.a = uint8_t(member->value.GetArray()[3].GetInt());
    }
    out.Write(binValue);
}

template<typename T>
inline void SerializeNumberArray(std::string_view name, const rapidjson::Value& value, BufferWriter& out)
{
    // Make a mutable copy initialized from the provided default.
    std::vector<T> binValue;

    // Create a span over the vector so ReadValueNumberArray can fill it.
    std::span<T> binSpan(binValue);

    // Read values from JSON into our span (will overwrite elements up to span.size()).
    ReadValueNumberArray<T>(name, binSpan, value);

    // Serialize the resulting numeric array to output.
    out.WriteArray<T>(binValue);
}

inline void SerializeColor(std::string_view name, const std::vector<uint8_t>& defaultValue, const rapidjson::Value& value, BufferWriter& out)
{
    // Make a mutable copy initialized from the provided default.
    std::vector<uint8_t> binValue = defaultValue;

    auto it = value.FindMember(name.data());
    if (it != value.MemberEnd() && it->value.IsArray())
    {
        const auto& valueArray = it->value;
        for (rapidjson::SizeType i = 0; i < valueArray.Size() && i < defaultValue.size(); ++i)
        {
            if (valueArray[i].IsNumber())
                binValue[i] = uint8_t(valueArray[i].GetUint());
        }
    }

    // Serialize the resulting numeric array to output.
    out.WriteArray<uint8_t>(binValue);
}

inline void SerializeRectangle(std::string_view name, const std::vector<float>& defaultValue, const rapidjson::Value& value, BufferWriter& out)
{
    // Make a mutable copy initialized from the provided default.
    std::vector<float> binValue = defaultValue;

    auto it = value.FindMember(name.data());
    if (it != value.MemberEnd() && it->value.IsArray())
    {
        const auto& valueArray = it->value;
        for (rapidjson::SizeType i = 0; i < valueArray.Size() && i < defaultValue.size(); ++i)
        {
            if (valueArray[i].IsNumber())
                binValue[i] = valueArray[i].GetFloat();
        }
    }

    // Serialize the resulting numeric array to output.
    out.WriteArray(binValue);
}

inline void SeralizeAssetReference(std::string_view name, const rapidjson::Value& value, BufferWriter& out)
{
    // Make a mutable copy initialized from the provided default.
    size_t nameValue = 0;

    auto it = value.FindMember(name.data());
    if (it != value.MemberEnd() && it->value.IsString())
    {
        nameValue = Hashes::CRC64Str(it->value.GetString());
    }

    // Serialize the resulting numeric array to output.
    out.Write<size_t>(nameValue);
}

template<typename T>
inline T ReadJsonMemberValue(std::string_view name, const rapidjson::Value& value, const T& defaultValue)
{
    T numberValue = defaultValue;
    auto it = value.FindMember(name.data());
    if (it != value.MemberEnd())
    {
        numberValue = static_cast<T>(it->value.Get<T>());
    }
    return numberValue;
}