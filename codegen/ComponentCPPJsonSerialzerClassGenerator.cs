using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection.Metadata;
using System.Text;

namespace codegen
{
    internal class ComponentCPPJsonSerialzerClassGenerator : ICppJsonSerializerClassGenerator
    {
        public void OutputClassInfo(StreamWriter writer, FileClasses classes)
        {
            foreach (var c in classes.Classes["component"])
            {
                writer.WriteLine($"inline void Serialize{c.Name}(const rapidjson::Value& json, BufferWriter& out)");
                writer.WriteLine("{");

                foreach (var field in c.Fields)
                {
                    WriteFieldSerializer(field, classes, writer);
                }
                writer.WriteLine("}");
                writer.WriteLine();
            }

            writer.WriteLine($"inline void RegisterCompoenents()");
            writer.WriteLine("{");
            foreach (var c in classes.Classes["component"])
            {
                writer.WriteLine($"\tComponentSerialization::Register(\"{c.Name}\",  Serialize{c.Name});");

            }
            writer.WriteLine("}");
        }

        internal static string GetFieldDefaultValue(FieldInfo field)
        {
            if(field.FieldTypename == "Vector2")
            {
                if (string.IsNullOrEmpty(field.DefaultValue))
                {
                    return "Vector2Zeroes";
                }

                var split = field.DefaultValue.TrimStart('{').TrimEnd('}').Split(',');
                return $"Vector2{{{split[0]}, {split[1]}}}";
            }
            
            return field.DefaultValue;
        }

        internal static void WriteFieldSerializer(FieldInfo field, FileClasses classes, StreamWriter writer)
        {
            string baseTypeName = GetFieldBaseType(field.FieldTypename);
            if (IsArrayType(field.FieldTypename))
            {
                writer.WriteLine($"\tSerializeNumberArray<{baseTypeName}>(\"{field.Name}\", {field.DefaultValue}, json, out);");
            }
            else if (IsNumberType(field.FieldTypename))
            {
                writer.WriteLine($"\tSerializeNumber<{GetCPPNumberType(baseTypeName)}>(\"{field.Name}\", {GetFieldDefaultValue(field)}, json, out);");
            }
            else if (IsAssetRef(field.FieldTypename))
            {
                writer.WriteLine($"\tSeralizeAssetReference(\"{field.Name}\", json, out);");
            }
            else if (IsColor(field.FieldTypename))
            {
                writer.WriteLine($"\tSerializeColor(\"{field.Name}\", {field.DefaultValue}, json, out);");
            }
            else if (IsColor(field.FieldTypename))
            {
                writer.WriteLine($"\tSerializeColor(\"{field.Name}\", {field.DefaultValue}, json, out);");
            }
            else // see if it's a known struct
            {
                foreach (var structClass in classes.Classes["struct"])
                {
                    if (structClass.Name == field.FieldTypename)
                    {
                        writer.WriteLine($"\tif (json.HasMember(\"{field.Name}\"))");
                        writer.WriteLine($"\t\tSerialize{structClass.Name}(json[\"{field.Name}\"], out);");
                    }
                }
            }
        }

        private static bool IsArrayType(string typeName)
        {
            return typeName.Contains("[") && typeName.Contains("]");
        }

        private static string GetFieldBaseType(string typeName)
        {
            if (IsArrayType(typeName))
            {
                return typeName.Substring(0, typeName.IndexOf('['));
            }
            return typeName;
        }

        private static bool IsNumberType(string typeName)
        {
            if (typeName.Contains("float"))
                return true;
            if (typeName.Contains("int32"))
                return true;
            if (typeName.Contains("Vector2"))
                return true;
            if (typeName.Contains("Color"))
                return true;
            return false;
        }

        private static string GetCPPNumberType(string typeName)
        {
            if (typeName.Contains("float"))
                return "float";
            if (typeName.Contains("int"))
                return "int32_t";

            return typeName;
        }

        private static bool IsAssetRef(string typeName)
        {
            if (typeName.Contains("AssetReference"))
                return true;

            return false;
        }

        private static bool IsColor(string typeName)
        {
            if (typeName.Contains("Color"))
                return true;

            return false;
        }

        public void OutputHeaderInfo(StreamWriter writer, FileClasses classes)
        {
            writer.WriteLine("#include \"ComponentSerialization.h\"");
        }
    }
}
