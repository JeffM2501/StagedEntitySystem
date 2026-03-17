using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Text;

namespace codegen
{
    internal interface ICppClassGenerator
    {
        public void OutputHeaderInfo(StreamWriter writer, FileClasses classes);
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

        internal static Dictionary<string, ICppClassGenerator> Generators = new Dictionary<string, ICppClassGenerator>();
        internal static void Init()
        {
            Generators.Add("message", new MessageCPPClassGenrator());
            Generators.Add("struct", new StructCPPClassGenerator());
            Generators.Add("component", new ComponentCPPClassGenerator());
        }
      
        public static void Generate(FileClasses fileClasses, string outputFile)
        {
            if (Generators.Count == 0)
                Init();

            File.Delete(outputFile);
            var writer = File.OpenWrite(outputFile);
            if (!writer.CanWrite)
                return;

            StreamWriter textWriter = new StreamWriter(writer);

            textWriter.WriteLine("#pragma once");
            textWriter.WriteLine();

            foreach ((var type, var list) in fileClasses.Classes)
            {
                if (Generators.ContainsKey(type))
                {
                    Generators[type].OutputHeaderInfo(textWriter, fileClasses);
                }
            }

            if (!string.IsNullOrEmpty(fileClasses.Namespace))
            {
                textWriter.WriteLine("namespace " + fileClasses.Namespace);
                textWriter.WriteLine("{");
            }

            foreach ((var type, var list) in fileClasses.Classes)
            {
                if (Generators.ContainsKey(type))
                {
                    Generators[type].OutputClassInfo(textWriter, fileClasses);
                }
            }

            if (!string.IsNullOrEmpty(fileClasses.Namespace))
            {
                textWriter.WriteLine("} //" + fileClasses.Namespace);
            }

            textWriter.Flush();
            textWriter.Close();
        }
    }
}
