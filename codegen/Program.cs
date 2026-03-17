using System;
using System.Collections.Generic;
using System.IO;

#nullable enable

namespace codegen
{
    class Program
    {
        static void ProcessSchemaFile(string? file, string outputDir, string outputType)
        {
            FileClasses fileClasses = new FileClasses();
            fileClasses.Parse(file);
            
            string? outputFileName = Path.GetFileNameWithoutExtension(file);

            if (outputFileName == null)
            {
                return;
            }

            string outPath = Path.Combine(outputDir, outputFileName + ".h");

            switch (outputType)
            {
                case "c++":
                    CPPGenerator.Generate(fileClasses, outPath);
                    break;

                default:
                    Console.WriteLine("Output mode " + outputDir + " is unknown");
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
            string OutputDir = args[2];

            if (string.IsNullOrEmpty(OutputDir) || !Directory.Exists(OutputDir))
            {
                Console.WriteLine("Output Dir " + OutputDir + " does not exit");
                return;
            }

            if (File.Exists(SchemaDir))
            {
                ProcessSchemaFile(SchemaDir, OutputDir, OutputType);
            }
            else if (Directory.Exists(SchemaDir)) 
            {
                foreach (var file in Directory.GetFiles(SchemaDir, "*.schema"))
                {
                    ProcessSchemaFile(file, OutputDir, OutputType);
                }
            }
            else
            {
                Console.WriteLine("Input " + SchemaDir + " is not a valid directory or file");
            }
        }
    }
}