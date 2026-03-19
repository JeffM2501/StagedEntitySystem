using System;
using System.Collections.Generic;
using System.IO;

#nullable enable

namespace codegen
{
    class Program
    {
        static void ProcessSchemaFile(string? file, string jsonOutputDir, string binaryOutputDir, string outputType)
        {
            FileClasses fileClasses = new FileClasses();
            fileClasses.Parse(file);
            
            string? outputFileName = Path.GetFileNameWithoutExtension(file);

            if (outputFileName == null)
            {
                return;
            }

            string jsonOutPath = Path.Combine(jsonOutputDir, outputFileName + ".h");
            string binaryOutPath = Path.Combine(binaryOutputDir, outputFileName + "Data_Generated.h");

            switch (outputType)
            {
                case "c++":
                    CPPGenerator.Generate(fileClasses, jsonOutPath, binaryOutPath);
                    break;

                default:
                    Console.WriteLine("Output mode " + jsonOutputDir + " is unknown");
                    break;
            }
        }

        static void Main(string[] args)
        {
            if (args.Length < 3)
            {
                Console.WriteLine("Missing args");
                return;
            }

            string OutputType = args[0];
            string SchemaDir = args[1];
            string JsonOutputDir = args[2];
            string BinaryOutputDir = args[3];

            if (string.IsNullOrEmpty(JsonOutputDir) || !Directory.Exists(JsonOutputDir))
            {
                Console.WriteLine("Output Dir " + JsonOutputDir + " does not exit");
                return;
            }

            if (File.Exists(SchemaDir))
            {
                ProcessSchemaFile(SchemaDir, JsonOutputDir, BinaryOutputDir, OutputType);
            }
            else if (Directory.Exists(SchemaDir)) 
            {
                foreach (var file in Directory.GetFiles(SchemaDir, "*.schema"))
                {
                    ProcessSchemaFile(file, JsonOutputDir, BinaryOutputDir, OutputType);
                }
            }
            else
            {
                Console.WriteLine("Input " + SchemaDir + " is not a valid directory or file");
            }
        }
    }
}