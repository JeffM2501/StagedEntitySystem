using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.CompilerServices;
using System.Text;
using System.Xml.Linq;

#nullable enable
namespace codegen
{
    internal class FieldInfo
    {
        public string Name = string.Empty;
        public string FieldTypename = string.Empty;
        public string DefaultValue = string.Empty;

        public Dictionary<string, string> Metadata = new Dictionary<string, string>();
    }

    internal class ClassInfo
    {
        public string Name = string.Empty;
        public string ClassType = string.Empty;

        public List<FieldInfo> Fields = new List<FieldInfo>();
    }

    internal class FileClasses
    {
        bool Valid = false;
        public string FileName = string.Empty;
        public string Namespace = string.Empty;
        public Dictionary<string, List<ClassInfo>> Classes = new Dictionary<string, List<ClassInfo>>();

        public List<string> Includes = new List<string>();

        public void Parse(string? filename)
        {
            if (string.IsNullOrEmpty(filename))
                return;

            string outputFileName = Path.GetFileNameWithoutExtension(filename);

            var reader = File.OpenText(filename);

            ClassInfo? currentClass = null;
            FieldInfo? currentField = null;

            var text = reader.ReadToEnd();
            text = text.Replace("\r", string.Empty);

            foreach (var line in text.Split(new char[] { '\n' }))
            {
                if (string.IsNullOrEmpty(line))
                    continue;

                var newLine = line.Trim();

                if (string.IsNullOrEmpty(newLine))
                    continue;

                var parts = newLine.Split(new char[] { ' ', '\t' }, 2);

                if (parts.Length < 1)
                    continue;

                var keyword = parts[0].ToLower();
                if (keyword == "namespace")
                {
                    Namespace = parts[1];
                }
                else if (keyword == "include")
                {
                    Includes.Add(parts[1].TrimStart('"').TrimEnd('"'));
                }
                else
                {
                    if (currentClass == null)
                    {
                        currentClass = new ClassInfo();

                        currentClass.ClassType = parts[0];
                        currentClass.Name = parts[1];
                    }
                    else
                    {
                        if (keyword == "}")
                        {
                            if (!Classes.ContainsKey(currentClass.ClassType))
                                Classes.Add(currentClass.ClassType, new List<ClassInfo>());
                            Classes[currentClass.ClassType].Add(currentClass);
        
                            currentClass = null;
                        }
                        else if (keyword != "{")
                        {
                            if (keyword.StartsWith("<"))
                            {
                                if (currentField == null)
                                    currentField = new FieldInfo();

                                var attribute = newLine.TrimStart('<').TrimEnd('>');
                                var attributeParts = attribute.Split(new char[] { ' ' },2);

                                currentField.Metadata.Add(attributeParts[0], attributeParts[1]);
                            }
                            else
                            {
                                if (currentField == null)
                                    currentField = new FieldInfo();

                                if (currentClass.ClassType != "enum")
                                {
                                    currentField.FieldTypename = parts[0].Trim();
                                    if (parts.Length > 1)
                                    {
                                        string name = parts[1].TrimEnd(';');
                                        if (name.Contains("="))
                                        {
                                            var nameParts = name.Split(new char[] { '=' });
                                            name = nameParts[0].Trim();
                                            currentField.DefaultValue = nameParts[1].Trim();
                                        }

                                        currentField.Name = name;
                                    }
                                }
                                else
                                {
                                    currentField.FieldTypename = "Enum";
                                    currentField.Name = parts[0].Trim(',');
                                }
                               
                                currentClass.Fields.Add(currentField);
                                currentField = null;
                            }
                        }
                    }
                }
            }

            Valid = Classes.Count > 0;
        }
    }
}
