using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Text;

namespace codegen
{
    internal interface ICppJsonSerializerClassGenerator
    {
        public void OutputHeaderInfo(StreamWriter writer, FileClasses classes);
        public void OutputClassInfo(StreamWriter writer, FileClasses classes);
    }

    internal interface ICppBinarySerializerClassGenerator
    {
        public void OutputClassInfo(StreamWriter writer, FileClasses classes);
    }

    internal static class CPPGenerator
    {
        internal static string GetCPPType(string typename)
        {
            switch(typename)
            {
                default:  return "uint32_t";

                case "float": return "float";
                case "double": return "double";
                case "u32": return "uint32_t";
                case "u64": return "uint64_t";
                case "string": return "std::string_view";
                case "bool": return "bool";
            }
        }

        internal static Dictionary<string, ICppJsonSerializerClassGenerator> JsonGenerators = new Dictionary<string, ICppJsonSerializerClassGenerator>();
        internal static Dictionary<string, ICppBinarySerializerClassGenerator> BinaryGenerators = new Dictionary<string, ICppBinarySerializerClassGenerator>();

        internal static void Init()
        {
            JsonGenerators.Add("message", new MessageCPPSerialzerClassGenrator());
            JsonGenerators.Add("struct", new StructCPPClassGenerator());
            JsonGenerators.Add("component", new ComponentCPPJsonSerialzerClassGenerator());

            BinaryGenerators.Add("component", new ComponentCPPBinarySerializerClassGenerator());
        }

        public static void GenerateJsonSerializer(FileClasses fileClasses, string jsonOutputFile)
        {
            File.Delete(jsonOutputFile);
            var writer = File.OpenWrite(jsonOutputFile);
            if (!writer.CanWrite)
                return;

            StreamWriter textWriter = new StreamWriter(writer);

            textWriter.WriteLine("#pragma once");
            textWriter.WriteLine();

            foreach ((var type, var list) in fileClasses.Classes)
            {
                if (JsonGenerators.ContainsKey(type))
                {
                    JsonGenerators[type].OutputHeaderInfo(textWriter, fileClasses);
                }
            }

            if (!string.IsNullOrEmpty(fileClasses.Namespace))
            {
                textWriter.WriteLine("namespace " + fileClasses.Namespace);
                textWriter.WriteLine("{");
            }

            foreach ((var type, var list) in fileClasses.Classes)
            {
                if (JsonGenerators.ContainsKey(type))
                {
                    JsonGenerators[type].OutputClassInfo(textWriter, fileClasses);
                }
            }

            if (!string.IsNullOrEmpty(fileClasses.Namespace))
            {
                textWriter.WriteLine("} //" + fileClasses.Namespace);
            }

            textWriter.Flush();
            textWriter.Close();
        }

        public static void GenerateBinarySerializer(FileClasses fileClasses, string outputFile)
        {
            File.Delete(outputFile);
            var writer = File.OpenWrite(outputFile);
            if (!writer.CanWrite)
                return;

            StreamWriter textWriter = new StreamWriter(writer);

            textWriter.WriteLine("#pragma once");

            textWriter.WriteLine("#include \"BufferReader.h\"");
            textWriter.WriteLine("#include \"SpriteManager.h\"");
            textWriter.WriteLine("#include \"raymath.h\"");
            textWriter.WriteLine();

            if (!string.IsNullOrEmpty(fileClasses.Namespace))
            {
                textWriter.WriteLine("namespace " + fileClasses.Namespace);
                textWriter.WriteLine("{");
            }

            foreach ((var type, var list) in fileClasses.Classes)
            {
                if (BinaryGenerators.ContainsKey(type))
                {
                    BinaryGenerators[type].OutputClassInfo(textWriter, fileClasses);
                }
            }

            if (!string.IsNullOrEmpty(fileClasses.Namespace))
            {
                textWriter.WriteLine("} //" + fileClasses.Namespace);
            }

            textWriter.Flush();
            textWriter.Close();
        }

        public static void Generate(FileClasses fileClasses, string jsonOutputFile, string binaryOutputDir)
        {
            if (JsonGenerators.Count == 0)
                Init();

            GenerateJsonSerializer(fileClasses, jsonOutputFile);
            GenerateBinarySerializer(fileClasses, binaryOutputDir);
        }
    }
}
