using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace codegen
{
    internal class ComponentCPPBinarySerializerClassGenerator : ICppBinarySerializerClassGenerator
    {
        string GetNativeType(FieldInfo field)
        {
            if (field.FieldTypename == "AssetReference")
                return "size_t";

            if (field.FieldTypename == "SpriteReference")
                return "SpriteInstance";

            if (field.FieldTypename == "int32")
                return "int32_t";

            return field.FieldTypename;
        }

        internal void WriteFieldData(StreamWriter writer, FieldInfo field)
        {
            if (field.Metadata.ContainsKey("NativeType"))
            {
                field.Metadata["NativeType"] = field.Metadata["NativeType"].Substring(2);

                if (field.Metadata["NativeType"] == "Vector2")
                {
                    writer.WriteLine($"\t{field.Metadata["NativeType"]} {field.Name} = {field.DefaultValue};");
                }
            }
            else if (field.FieldTypename == "SpriteReference")
            {
                writer.WriteLine($"\tSpriteManager::SpriteInstance {field.Name};");
            }
            else
            {
                string defaultValue = string.Empty;
                if (!string.IsNullOrEmpty(field.DefaultValue))
                {
                    defaultValue = " = " + field.DefaultValue;
                }
                writer.WriteLine($"\t{GetNativeType(field)} {field.Name}{defaultValue};");
            }
        }

        internal void WriteFieldReader(StreamWriter writer, FieldInfo field)
        {
            if (field.Metadata.ContainsKey("NativeType"))
            {
                if (field.Metadata["NativeType"] == "Vector2")
                {
                    writer.WriteLine($"\t\t{field.Name} = buffer.Read<Vector2>();");
                }
            }
            else if (field.FieldTypename == "SpriteReference")
            {
                writer.WriteLine($"\t\t{field.Name} = SpriteManager::LoadFromBuffer(buffer);");
            }
            else
            {
                writer.WriteLine($"\t\t{field.Name} = buffer.Read<{GetNativeType(field)}>();");
            }
        }

        public void OutputClassInfo(StreamWriter writer, FileClasses classes)
        {
            foreach (var c in classes.Classes["component"])
            {
                writer.WriteLine($"struct {c.Name}Data");
                writer.WriteLine("{");

                // write values
                foreach (var field in c.Fields)
                {
                    WriteFieldData(writer, field);
                }
                writer.WriteLine();

                writer.WriteLine("\tvoid Read(BufferReader& buffer)");
                writer.WriteLine("\t{");
                foreach (var field in c.Fields)
                {
                    WriteFieldReader(writer, field);
                }
                writer.WriteLine("\t}");

                writer.WriteLine("};");
                writer.WriteLine();
            }
        }
    }
}
