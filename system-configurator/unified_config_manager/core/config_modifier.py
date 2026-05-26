"""
Dynamic Configuration Modifier

Functions for adding, deleting, duplicating, and reordering configs in .h files.
Preserves file format, comments, spacing, and indentation.
"""

import re
from pathlib import Path
from typing import Any, Optional
from .models import ConfigType
from .config_io import load_file, save_file


def add_config_to_file(
    filepath: Path,
    section_name: str,
    config_name: str,
    config_value: Any,
    config_type: ConfigType,
    description: str,
    comment: str = ""
) -> None:
    """
    เพิ่ม config ใหม่ลงในไฟล์ .h
    
    Args:
        filepath: Path to .h file
        section_name: Section name to add config to (can include emoji and Thai text)
        config_name: Name of new config (e.g., "ENABLE_NEW_FEATURE")
        config_value: Default value
        config_type: ConfigType (TOGGLE, NUMBER, STRING)
        description: Brief description
        comment: Optional detailed comment
    """
    content = load_file(filepath)
    lines = content.split('\n')
    
    # Format value based on type
    if config_type == ConfigType.TOGGLE:
        formatted_value = '1' if bool(config_value) else '0'
    elif config_type == ConfigType.STRING:
        formatted_value = f'"{config_value}"'
    else:
        formatted_value = str(config_value)
    
    # Extract English part from section name (remove emoji and Thai text)
    # e.g., "🔌 HARDWARE MODULES - โมดูลฮาร์ดแวร์" -> "HARDWARE MODULES"
    section_search = section_name
    if ' - ' in section_name:
        section_search = section_name.split(' - ')[0]  # Remove Thai part
    # Remove emoji and extra spaces
    section_search = re.sub(r'[^\w\s]', '', section_search).strip().upper()
    
    # Find section
    section_start = -1
    section_end = -1
    section_header_end = -1
    
    # Look for section comment block
    for i, line in enumerate(lines):
        # Match section headers like "/* ===== MODULE CONFIGURATIONS ===== */"
        # Use fuzzy matching - check if section_search words are in the line
        if '/*' in line or '//' in line:
            line_upper = line.upper()
            # Check if all words from section_search are in the line
            words = section_search.split()
            if all(word in line_upper for word in words):
                section_start = i
                
                # Find end of section header (skip all comment lines and empty lines)
                section_header_end = i
                for k in range(i + 1, len(lines)):
                    if lines[k].strip().startswith('/*') or lines[k].strip().startswith('*') or lines[k].strip() == '':
                        section_header_end = k
                    else:
                        break
                
                # Find end of section (next section or end of file)
                for j in range(section_header_end + 1, len(lines)):
                    # Next section starts
                    if re.match(r'^\s*/\*\s*=+.*=+\s*\*/', lines[j]):
                        section_end = j
                        break
                if section_end == -1:
                    # No next section found, look for #endif
                    for j in range(section_header_end + 1, len(lines)):
                        if '#endif' in lines[j]:
                            section_end = j
                            break
                    if section_end == -1:
                        section_end = len(lines)
                break
    
    if section_start == -1:
        raise ValueError(
            f"Section matching '{section_search}' not found in {filepath}\n"
            f"Original section name: '{section_name}'"
        )
    
    # Create new config lines
    new_lines = []
    if comment:
        # Multi-line comment
        new_lines.append(f"/* {comment} */")
    new_lines.append(f"#define {config_name}  {formatted_value}  // {description}")
    new_lines.append("")  # Empty line after
    
    # Find the last config in the section to insert after it
    last_config_line = section_header_end
    for i in range(section_header_end + 1, section_end):
        if re.match(r'^\s*#define\s+\w+', lines[i]):
            last_config_line = i
    
    # Insert after the last config (or after section header if no configs)
    insert_pos = last_config_line + 1
    
    for line_text in reversed(new_lines):
        lines.insert(insert_pos, line_text)
    
    # Write back
    new_content = '\n'.join(lines)
    save_file(filepath, new_content)


def delete_config_from_file(
    filepath: Path,
    config_name: str
) -> None:
    """
    ลบ config จากไฟล์ .h
    
    Args:
        filepath: Path to .h file
        config_name: Name of config to delete
    """
    content = load_file(filepath)
    lines = content.split('\n')
    
    # Find and remove config line
    new_lines = []
    skip_next_empty = False
    
    for i, line in enumerate(lines):
        # Match #define CONFIG_NAME
        if re.match(rf'^\s*(//\s*)?#define\s+{config_name}\b', line):
            # Check if previous line is a comment for this config
            if i > 0 and ('/*' in lines[i-1] or '//' in lines[i-1]):
                # Remove previous comment line too
                new_lines.pop()
            skip_next_empty = True
            continue  # Skip this line
        
        # Skip empty line after deleted config
        if skip_next_empty and line.strip() == '':
            skip_next_empty = False
            continue
        
        new_lines.append(line)
    
    # Write back
    new_content = '\n'.join(new_lines)
    save_file(filepath, new_content)


def add_section_to_file(
    filepath: Path,
    section_name: str,
    description: str = ""
) -> None:
    """
    เพิ่ม section ใหม่
    
    Args:
        filepath: Path to .h file
        section_name: Name of new section
        description: Optional description
    """
    content = load_file(filepath)
    lines = content.split('\n')
    
    # Create section header
    section_header = [
        "",
        f"/* {'=' * 60} */",
        f"/* {section_name.upper().center(58)} */",
    ]
    
    if description:
        section_header.append(f"/* {description.center(58)} */")
    
    section_header.append(f"/* {'=' * 60} */")
    section_header.append("")
    
    # Find last section or end of file
    insert_pos = len(lines)
    
    # Look for #endif at end
    for i in range(len(lines) - 1, -1, -1):
        if '#endif' in lines[i]:
            insert_pos = i
            break
    
    # Insert section header
    for j, line in enumerate(section_header):
        lines.insert(insert_pos + j, line)
    
    # Write back
    new_content = '\n'.join(lines)
    save_file(filepath, new_content)
