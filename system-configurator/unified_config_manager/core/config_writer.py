"""
Configuration file writers for C header files.

Writers for modules, params, library, and ksdef configuration files.
All regex patterns and logic are preserved exactly from the original implementation.
"""

import re
from typing import Dict, Any
from .models import ConfigFile, ConfigType


def write_modules_config(config_file: ConfigFile, updates: Dict[str, Any]) -> str:
    """
    Write modules configuration updates back to content.
    
    Args:
        config_file: ConfigFile object containing original content
        updates: Dictionary of {config_name: new_value} updates to apply
        
    Returns:
        Updated content string
        
    Note:
        Only applies updates for configs starting with 'modules_' prefix
    """
    content = config_file.original_content
    
    for key, value in updates.items():
        if key.startswith('modules_'):
            name = key[len('modules_'):]  # Remove 'modules_' prefix
            
            # Check config type to determine how to format value
            config = config_file.get_config(name)
            if config and config.config_type == ConfigType.TOGGLE:
                # Toggle: format as 0 or 1
                formatted_value = '1' if bool(value) else '0'
            else:
                # Number: use as-is
                formatted_value = str(value)
            
            # Exact regex pattern from original: (#define\s+{name}\s+)\d+
            content = re.sub(
                rf'(#define\s+{name}\s+)\d+',
                rf'\g<1>{formatted_value}',
                content
            )
    
    return content


def write_params_config(config_file: ConfigFile, updates: Dict[str, Any]) -> str:
    """
    Write params configuration updates back to content.
    
    Args:
        config_file: ConfigFile object containing original content
        updates: Dictionary of {config_name: new_value} updates to apply
        
    Returns:
        Updated content string
        
    Note:
        Only applies updates for configs starting with 'params_' prefix
        Handles both toggle and number types
    """
    content = config_file.original_content
    
    for key, value in updates.items():
        if key.startswith('params_'):
            name = key[len('params_'):]  # Remove 'params_' prefix
            
            # Check config type to determine how to format value
            config = config_file.get_config(name)
            if config:
                if config.config_type == ConfigType.TOGGLE:
                    # Toggle: format as 0 or 1
                    formatted_value = '1' if bool(value) else '0'
                elif config.config_type == ConfigType.STRING:
                    # String: add quotes
                    formatted_value = f'"{value}"'
                else:
                    # Number: use as-is
                    formatted_value = str(value)
                
                # Exact regex pattern from original: (#define\s+{name}\s+)\d+
                # For strings, we use a different pattern
                if config.config_type == ConfigType.STRING:
                    content = re.sub(
                        rf'#define\s+{name}\s+"[^"]+"',
                        rf'#define {name} {formatted_value}',
                        content
                    )
                else:
                    content = re.sub(
                        rf'(#define\s+{name}\s+)\d+',
                        rf'\g<1>{formatted_value}',
                        content
                    )
    
    return content


def write_library_config(config_file: ConfigFile, updates: Dict[str, Any]) -> str:
    """
    Write library configuration updates back to content.
    
    Args:
        config_file: ConfigFile object containing original content
        updates: Dictionary of {config_name: new_value} updates to apply
        
    Returns:
        Updated content string
        
    Note:
        Only applies updates for configs starting with 'library_' prefix
        Handles toggle, number, and string types
    """
    content = config_file.original_content
    
    for key, value in updates.items():
        if key.startswith('library_'):
            name = key[len('library_'):]  # Remove 'library_' prefix
            
            # Check config type to determine how to format value
            config = config_file.get_config(name)
            if config:
                if config.config_type == ConfigType.TOGGLE:
                    # Toggle: format as 0 or 1
                    formatted_value = '1' if bool(value) else '0'
                elif config.config_type == ConfigType.STRING:
                    # String: add quotes
                    formatted_value = f'"{value}"'
                else:
                    # Number: use as-is
                    formatted_value = str(value)
                
                # Exact regex pattern from original
                if config.config_type == ConfigType.STRING:
                    content = re.sub(
                        rf'#define\s+{name}\s+"[^"]+"',
                        rf'#define {name} "{value}"',
                        content
                    )
                else:
                    content = re.sub(
                        rf'(#define\s+{name}\s+)\d+',
                        rf'\g<1>{formatted_value}',
                        content
                    )
    
    return content
    
    return content


def write_ksdef_config(config_file: ConfigFile, updates: Dict[str, Any]) -> str:
    """
    Write ksdef.h configuration updates back to content.
    
    Special handling for commented/uncommented defines.
    
    Args:
        config_file: ConfigFile object containing original content
        updates: Dictionary of {config_name: new_value} updates to apply
        
    Returns:
        Updated content string
        
    Note:
        Only applies updates for configs starting with 'ksdef_' prefix
        Handles toggle (comment/uncomment), number, and string types
    """
    content = config_file.original_content
    lines = content.split('\n')
    
    for key, value in updates.items():
        if key.startswith('ksdef_'):
            name = key[len('ksdef_'):]  # Remove 'ksdef_' prefix
            
            # Check config type to determine how to format value
            config = config_file.get_config(name)
            if not config:
                continue
            
            if config.config_type == ConfigType.TOGGLE:
                # Toggle: comment/uncomment the define
                for i, line in enumerate(lines):
                    # Exact regex pattern from original: ^\s*(//\s*)?#define\s+{name}\b
                    if re.match(rf'^\s*(//\s*)?#define\s+{name}\b', line):
                        is_commented = '//' in line
                        should_enable = bool(value)
                        
                        if should_enable and is_commented:
                            # Uncomment: remove leading //
                            lines[i] = re.sub(rf'^\s*//\s*', '', line)
                        elif not should_enable and not is_commented:
                            # Comment: add leading //
                            lines[i] = '//' + line.lstrip()
                        break
                            
            elif config.config_type == ConfigType.NUMBER:
                # Number: update the value
                formatted_value = str(value)
                for i, line in enumerate(lines):
                    # Exact regex pattern from original: ^\s*(//\s*)?#define\s+{name}\s+\d+
                    if re.match(rf'^\s*(//\s*)?#define\s+{name}\s+\d+', line):
                        lines[i] = f'#define {name} {formatted_value}'
                        break
                        
            elif config.config_type == ConfigType.STRING:
                # String: update the value
                formatted_value = f'"{value}"'
                for i, line in enumerate(lines):
                    # Exact regex pattern from original: ^\s*(//\s*)?#define\s+{name}\s+"[^"]*"
                    if re.match(rf'^\s*(//\s*)?#define\s+{name}\s+"[^"]*"', line):
                        lines[i] = f'#define {name} {formatted_value}'
                        break
    
    return '\n'.join(lines)
