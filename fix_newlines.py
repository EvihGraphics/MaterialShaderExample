import re
with open('Plugins/MaterialShaderPlugin/Source/MaterialShaderPlugin/Private/AVBOIT/Testing/AVBOITBackendTestAutomation.cpp', 'r', encoding='utf-8') as f:
    content = f.read()

content = content.replace('MeasuredT\n");', 'MeasuredT\\n");')
content = content.replace('MeasuredT\n"),', 'MeasuredT\\n"),')
content = content.replace('{\n");', '{\\n");')
content = content.replace('Cases": %d,\n"),', 'Cases\\": %d,\\n"),')
content = content.replace('Cases": [\n");', 'Cases\\": [\\n");')
content = content.replace('  {\n");', '  {\\n");')
content = content.replace('"Name": "%s",\n"),', '\\"Name\\": \\"%s\\",\\n"),')
content = content.replace('"Status": "%s",\n"),', '\\"Status\\": \\"%s\\",\\n"),')
content = content.replace(']\n  }");', ']\\n  }");')
content = content.replace('\n");\n    }', '\\n");\n    }')
content = content.replace(']\n}\n");', ']\\n}\\n");')

with open('Plugins/MaterialShaderPlugin/Source/MaterialShaderPlugin/Private/AVBOIT/Testing/AVBOITBackendTestAutomation.cpp', 'w', encoding='utf-8') as f:
    f.write(content)
