using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection.Metadata;
using System.Text;

namespace codegen
{
    internal class StructCPPClassGenerator : ICppJsonSerializerClassGenerator
    {
        public void OutputClassInfo(StreamWriter writer, FileClasses classes)
        {
            foreach (var c in classes.Classes["struct"])
            {
                writer.WriteLine($"inline void Serialize{c.Name}(const rapidjson::Value& json, BufferWriter& out)");
                writer.WriteLine("{");

                foreach (var field in c.Fields)
                {
                    ComponentCPPJsonSerialzerClassGenerator.WriteFieldSerializer(field, classes, writer);
                }
                writer.WriteLine("}");
                writer.WriteLine();
            }

        }

        public void OutputHeaderInfo(StreamWriter writer, FileClasses classes)
        {
          /*  writer.WriteLine("#include \"ComponentSerialization.h\"");*/
        }
    }
}
