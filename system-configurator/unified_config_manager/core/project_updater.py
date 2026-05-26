"""
Project file updater for Eclipse .cproject files.

Updates .cproject files with new include paths using text-based parsing.
Preserves existing configuration and formatting.
"""

from pathlib import Path
import re
from .config_io import FileOperationError


class ProjectUpdateError(Exception):
    """Exception raised when .cproject file update fails."""
    pass


def _extract_include_paths_from_template(template_path: Path) -> tuple[list[str], list[str]]:
    """
    Extract include paths from template file using regex.
    
    Returns:
        Tuple of (assembler_paths, compiler_paths)
    """
    try:
        with open(template_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        assembler_paths = []
        compiler_paths = []
        
        # Find all tool sections
        tool_pattern = r'<tool id="([^"]*)" name="([^"]*)"[^>]*>(.*?)</tool>'
        tools = re.findall(tool_pattern, content, re.DOTALL)
        
        for tool_id, tool_name, tool_content in tools:
            # Check if this is assembler or compiler
            is_assembler = 'assembler' in tool_name.lower() or 'assembler' in tool_id.lower()
            is_compiler = 'compiler' in tool_name.lower() and 'c++' not in tool_name.lower()
            
            if not (is_assembler or is_compiler):
                continue
            
            # Find include paths option
            option_pattern = r'<option[^>]*name="Include paths \(-I\)"[^>]*>(.*?)</option>'
            option_match = re.search(option_pattern, tool_content, re.DOTALL)
            
            if option_match:
                option_content = option_match.group(1)
                # Extract all listOptionValue elements
                value_pattern = r'<listOptionValue[^>]*value="([^"]*)"'
                values = re.findall(value_pattern, option_content)
                
                for value in values:
                    if value and value.strip():
                        if is_assembler:
                            if value not in assembler_paths:
                                assembler_paths.append(value)
                        elif is_compiler:
                            if value not in compiler_paths:
                                compiler_paths.append(value)
        
        return assembler_paths, compiler_paths
        
    except Exception as e:
        raise ProjectUpdateError(f"Error parsing template file: {str(e)}") from e


def _extract_source_entries_from_template(template_path: Path) -> list[tuple[str, str]]:
    """
    Extract source entries from template file.
    
    Returns:
        List of (name, excluding) tuples. excluding is empty string if not present.
    """
    try:
        with open(template_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        source_entries = []
        
        # Find sourceEntries sections (there may be multiple configurations)
        source_entries_pattern = r'<sourceEntries>(.*?)</sourceEntries>'
        sections = re.findall(source_entries_pattern, content, re.DOTALL)
        
        if not sections:
            return source_entries
        
        # Use the first sourceEntries section (Debug configuration)
        first_section = sections[0]
        
        # Extract entry elements
        # <entry flags="VALUE_WORKSPACE_PATH|RESOLVED" kind="sourcePath" name="Applications"/>
        # <entry excluding="examples" flags="VALUE_WORKSPACE_PATH|RESOLVED" kind="sourcePath" name="Configuration"/>
        entry_pattern = r'<entry\s+([^>]*)/>'
        entries = re.findall(entry_pattern, first_section)
        
        for entry_attrs in entries:
            # Extract name attribute
            name_match = re.search(r'name="([^"]*)"', entry_attrs)
            if not name_match:
                continue
            
            name = name_match.group(1)
            
            # Extract excluding attribute if present
            excluding_match = re.search(r'excluding="([^"]*)"', entry_attrs)
            excluding = excluding_match.group(1) if excluding_match else ''
            
            source_entries.append((name, excluding))
        
        return source_entries
        
    except Exception as e:
        raise ProjectUpdateError(f"Error extracting source entries: {str(e)}") from e


def _merge_source_entries_text(cproject_path: Path, source_entries: list[tuple[str, str]]) -> bool:
    """
    Merge source entries into existing .cproject file.
    
    Args:
        cproject_path: Path to .cproject file
        source_entries: List of (name, excluding) tuples to add
        
    Returns:
        True if changes were made
    """
    try:
        with open(cproject_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        original_content = content
        added_count = 0
        
        # Find all sourceEntries sections
        source_entries_pattern = r'(<sourceEntries>)(.*?)(</sourceEntries>)'
        
        def replace_source_entries(match):
            nonlocal added_count
            
            start_tag = match.group(1)
            entries_content = match.group(2)
            end_tag = match.group(3)
            
            # Extract existing entry names
            existing_names = set()
            entry_pattern = r'<entry\s+[^>]*name="([^"]*)"[^>]*/>'
            for name in re.findall(entry_pattern, entries_content):
                existing_names.add(name)
            
            # Build new entries to add
            new_entries = []
            for name, excluding in source_entries:
                if name not in existing_names:
                    if excluding:
                        new_entries.append(
                            f'\n\t\t\t\t\t<entry excluding="{excluding}" flags="VALUE_WORKSPACE_PATH|RESOLVED" kind="sourcePath" name="{name}"/>'
                        )
                    else:
                        new_entries.append(
                            f'\n\t\t\t\t\t<entry flags="VALUE_WORKSPACE_PATH|RESOLVED" kind="sourcePath" name="{name}"/>'
                        )
                    added_count += 1
            
            if not new_entries:
                return match.group(0)
            
            # Find the last entry to insert after it
            last_entry_match = None
            for m in re.finditer(r'<entry\s+[^>]*/>', entries_content):
                last_entry_match = m
            
            if last_entry_match:
                # Insert after the last entry
                insert_pos = last_entry_match.end()
                new_entries_content = (
                    entries_content[:insert_pos] +
                    ''.join(new_entries) +
                    entries_content[insert_pos:]
                )
            else:
                # No existing entries, add after start tag
                new_entries_content = ''.join(new_entries) + '\n\t\t\t\t' + entries_content
            
            return start_tag + new_entries_content + end_tag
        
        # Replace all sourceEntries sections
        new_content = re.sub(source_entries_pattern, replace_source_entries, content, flags=re.DOTALL)
        
        # Write back if changes were made
        if new_content != original_content:
            with open(cproject_path, 'w', encoding='utf-8') as f:
                f.write(new_content)
            
            print(f"  Added {added_count} new source entries")
            return True
        
        return False
        
    except Exception as e:
        raise ProjectUpdateError(f"Error merging source entries: {str(e)}") from e


def _merge_include_paths_text(cproject_path: Path, assembler_paths: list[str], compiler_paths: list[str]) -> bool:
    """
    Merge include paths into existing .cproject file using text manipulation.
    Preserves formatting and existing configuration.
    
    Args:
        cproject_path: Path to .cproject file
        assembler_paths: List of assembler include paths to add
        compiler_paths: List of compiler include paths to add
        
    Returns:
        True if changes were made
    """
    try:
        with open(cproject_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        original_content = content
        added_assembler = 0
        added_compiler = 0
        
        # Find all tool sections
        tool_pattern = r'(<tool id="([^"]*)" name="([^"]*)"[^>]*>)(.*?)(</tool>)'
        
        def replace_tool(match):
            nonlocal added_assembler, added_compiler
            
            tool_start = match.group(1)
            tool_id = match.group(2)
            tool_name = match.group(3)
            tool_content = match.group(4)
            tool_end = match.group(5)
            
            # Check if this is assembler or compiler
            is_assembler = 'assembler' in tool_name.lower() or 'assembler' in tool_id.lower()
            is_compiler = 'compiler' in tool_name.lower() and 'c++' not in tool_name.lower()
            
            if not (is_assembler or is_compiler):
                return match.group(0)
            
            # Find include paths option
            option_pattern = r'(<option[^>]*name="Include paths \(-I\)"[^>]*>)(.*?)(</option>)'
            option_match = re.search(option_pattern, tool_content, re.DOTALL)
            
            if not option_match:
                return match.group(0)
            
            option_start = option_match.group(1)
            option_content = option_match.group(2)
            option_end = option_match.group(3)
            
            # Get existing paths
            value_pattern = r'<listOptionValue[^>]*value="([^"]*)"'
            existing_paths = set(re.findall(value_pattern, option_content))
            
            # Determine which paths to add
            if is_assembler:
                paths_to_add = [p for p in assembler_paths if p not in existing_paths]
            else:
                paths_to_add = [p for p in compiler_paths if p not in existing_paths]
            
            if not paths_to_add:
                return match.group(0)
            
            # Build new listOptionValue elements with proper formatting
            new_values = []
            for path in paths_to_add:
                new_values.append(f'\n\t\t\t\t\t<listOptionValue builtIn="false" value="{path}"/>')
                if is_assembler:
                    added_assembler += 1
                else:
                    added_compiler += 1
            
            # Insert new values before the closing </option> tag
            # Find the last listOptionValue to maintain formatting
            last_value_match = None
            for m in re.finditer(r'<listOptionValue[^>]*/>', option_content):
                last_value_match = m
            
            if last_value_match:
                # Insert after the last listOptionValue
                insert_pos = last_value_match.end()
                new_option_content = (
                    option_content[:insert_pos] + 
                    ''.join(new_values) + 
                    option_content[insert_pos:]
                )
            else:
                # No existing values, add after option start
                new_option_content = ''.join(new_values) + '\n\t\t\t\t' + option_content
            
            # Rebuild the option
            new_option = option_start + new_option_content + option_end
            
            # Rebuild the tool
            new_tool_content = tool_content.replace(option_match.group(0), new_option)
            
            return tool_start + new_tool_content + tool_end
        
        # Replace all tools
        new_content = re.sub(tool_pattern, replace_tool, content, flags=re.DOTALL)
        
        # Write back if changes were made
        if new_content != original_content:
            with open(cproject_path, 'w', encoding='utf-8') as f:
                f.write(new_content)
            
            print(f"  Added {added_assembler} new assembler include paths")
            print(f"  Added {added_compiler} new compiler include paths")
            return True
        
        return False
        
    except Exception as e:
        raise ProjectUpdateError(f"Error merging include paths: {str(e)}") from e


def update_cproject_file(export_path: Path, template_path: Path = None, mode: str = 'merge') -> bool:
    """
    Update .cproject file with new include paths and source entries.
    
    Args:
        export_path: Path to project directory
        template_path: Path to .cproject template file (optional)
        mode: 'merge' to merge paths, 'replace' to replace entire file (default: 'merge')
        
    Returns:
        True if successful
        
    Raises:
        ProjectUpdateError: If .cproject cannot be found/updated
    """
    cproject_path = export_path / '.cproject'
    
    if not cproject_path.exists():
        raise ProjectUpdateError(
            f".cproject file not found at: {cproject_path}\n"
            "Cannot update project configuration"
        )
    
    # If no template provided, look for .cproject_configafter in current directory
    if template_path is None:
        # Try to find template in the same directory as this script
        script_dir = Path(__file__).parent.parent.parent  # Go up to project root
        template_path = script_dir / '.cproject_configafter'
        
        if not template_path.exists():
            # Fallback to using existing .cproject (no update)
            print(f"Warning: Template file not found at: {template_path}")
            print(f"Using existing .cproject without updates")
            return True
    elif not template_path.exists():
        # Fallback to using existing .cproject
        print(f"Warning: Template file not found at: {template_path}")
        print(f"Using existing .cproject without updates")
        return True
    
    try:
        if mode == 'replace':
            # Old behavior: Replace entire file
            with open(template_path, 'r', encoding='utf-8') as f:
                template_content = f.read()
            
            with open(cproject_path, 'w', encoding='utf-8') as f:
                f.write(template_content)
            
            print(f"Successfully replaced .cproject file using template: {template_path.name}")
        else:
            # New behavior: Merge include paths and source entries using text manipulation
            print(f"Merging configuration from template: {template_path.name}")
            
            # Extract from template
            assembler_paths, compiler_paths = _extract_include_paths_from_template(template_path)
            source_entries = _extract_source_entries_from_template(template_path)
            
            print(f"  Found {len(assembler_paths)} assembler paths in template")
            print(f"  Found {len(compiler_paths)} compiler paths in template")
            print(f"  Found {len(source_entries)} source entries in template")
            
            # Merge include paths
            paths_changed = _merge_include_paths_text(cproject_path, assembler_paths, compiler_paths)
            
            # Merge source entries
            sources_changed = _merge_source_entries_text(cproject_path, source_entries)
            
            if paths_changed or sources_changed:
                print(f"Successfully merged configuration into: {cproject_path.name}")
            else:
                print(f"No new configuration to add (all entries already exist)")
        
        return True
        
    except Exception as e:
        raise ProjectUpdateError(
            f"Error updating .cproject file: {str(e)}"
        ) from e
