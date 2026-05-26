#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Unified Configuration Manager - CLI
A command-line interface for managing embedded system configurations.
"""

import sys
import argparse
import os
from typing import List, Tuple, Optional

# Add parent directory to path to allow absolute imports if needed
# But normally this will be used as a package: unified_config_manager.cli.main_cli
try:
    from ..core import (
        parse_modules_config,
        parse_params_config,
        parse_library_config,
        parse_ksdef_config,
        write_modules_config,
        write_params_config,
        write_library_config,
        write_ksdef_config,
        export_all_configs,
        ConfigFile,
        ConfigItem,
        ToggleConfig,
        NumberConfig,
        StringConfig
    )
except (ImportError, ValueError):
    # Fallback for direct execution
    sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../..')))
    from unified_config_manager.core import (
        parse_modules_config,
        parse_params_config,
        parse_library_config,
        parse_ksdef_config,
        write_modules_config,
        write_params_config,
        write_library_config,
        write_ksdef_config,
        export_all_configs,
        ConfigFile,
        ConfigItem,
        ToggleConfig,
        NumberConfig,
        StringConfig
    )

def load_all_configs(fw_src: Optional[str] = None) -> List[ConfigFile]:
    """Load all configuration files."""
    configs = []
    
    # Base paths
    base_path = os.path.join(fw_src, 'Configuration') if fw_src else 'Configuration'
    
    try:
        # Try to load each, handle if file not found
        try:
            configs.append(parse_modules_config(os.path.join(base_path, 'app_modules_config.h')))
        except Exception as e:
            print(f"Warning: Could not load modules config: {e}")
        
        try:
            configs.append(parse_params_config(os.path.join(base_path, 'csp_app_params.h')))
        except Exception as e:
            print(f"Warning: Could not load params config: {e}")
        
        try:
            configs.append(parse_library_config(os.path.join(base_path, 'csp_library_config.h')))
        except Exception as e:
            print(f"Warning: Could not load library config: {e}")
        
        try:
            configs.append(parse_ksdef_config(os.path.join(base_path, 'ksdef.h')))
        except Exception as e:
            print(f"Warning: Could not load ksdef config: {e}")
        
        if not configs:
            print("Error: No configuration files found or loaded.")
            sys.exit(1)
            
        return configs
    except Exception as e:
        print(f"Error loading configurations: {e}")
        sys.exit(1)

def find_config_in_all(configs: List[ConfigFile], name: str) -> Tuple[Optional[ConfigFile], Optional[ConfigItem]]:
    """Find a config item by name across all loaded files."""
    for config_file in configs:
        item = config_file.get_config(name)
        if item:
            return config_file, item
    return None, None

def print_item(item: ConfigItem, show_desc: bool = True):
    """Print a single configuration item with formatting."""
    val = item.value
    if isinstance(item, ToggleConfig):
        val = 1 if item.value else 0
    
    output = f"{item.name} = {val}"
    if show_desc:
        output += f" # {item.description}"
    print(output)

def main():
    parser = argparse.ArgumentParser(
        description="Unified Configuration Manager CLI",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  python unified_config_manager_cli.py list --values
  python unified_config_manager_cli.py get ENABLE_HW_CAN
  python unified_config_manager_cli.py set ENABLE_HW_CAN 1
  python unified_config_manager_cli.py export
        """
    )
    
    subparsers = parser.add_subparsers(dest="command", help="Commands")

    # List command
    list_parser = subparsers.add_parser("list", help="List configuration items")
    list_parser.add_argument("--fw-src", help="Source framework root directory (optional)")
    list_parser.add_argument("--sections", action="store_true", help="Group by sections")
    list_parser.add_argument("--values", action="store_true", help="Show current values")
    list_parser.add_argument("--no-desc", action="store_true", help="Don't show descriptions")

    # Get command
    get_parser = subparsers.add_parser("get", help="Get a configuration value")
    get_parser.add_argument("name", help="Name of the configuration item")
    get_parser.add_argument("--fw-src", help="Source framework root directory (optional)")

    # Set command
    set_parser = subparsers.add_parser("set", help="Set a configuration value")
    set_parser.add_argument("name", help="Name of the configuration item")
    set_parser.add_argument("value", help="New value for the configuration")
    set_parser.add_argument("--fw-src", help="Source framework root directory (optional)")

    # Export command
    export_parser = subparsers.add_parser("export", help="Export all configurations to project files")
    export_parser.add_argument("dest", nargs="?", default="exported_configs", help="Destination directory for export (default: exported_configs)")
    export_parser.add_argument("--fw-src", help="Source framework root directory (optional)")

    args = parser.parse_args()

    fw_src = getattr(args, 'fw_src', None)

    if args.command == "list":
        configs = load_all_configs(fw_src)
        for config_file in configs:
            print(f"\n[{os.path.basename(config_file.filepath)}]")
            for section in config_file.sections:
                if args.sections:
                    print(f"\n  ## {section.name}")
                for item in section.configs:
                    if args.values:
                        print_item(item, not args.no_desc)
                    else:
                        print(f"  {item.name}")
                        
    elif args.command == "get":
        configs = load_all_configs(fw_src)
        _, item = find_config_in_all(configs, args.name)
        if item:
            print_item(item)
        else:
            print(f"Error: Config '{args.name}' not found.")
            sys.exit(1)

    elif args.command == "set":
        configs = load_all_configs(fw_src)
        config_file, item = find_config_in_all(configs, args.name)
        if item:
            try:
                # Handle boolean/toggle values
                if isinstance(item, ToggleConfig):
                    val_str = str(args.value).lower()
                    if val_str in ('1', 'true', 'on', 'yes', 'enable'):
                        item.value = True
                    elif val_str in ('0', 'false', 'off', 'no', 'disable'):
                        item.value = False
                    else:
                        print(f"Error: Invalid toggle value: {args.value}. Use 1/0, true/false, or enable/disable.")
                        sys.exit(1)
                
                # Handle numeric values
                elif isinstance(item, NumberConfig):
                    item.value = int(args.value)
                
                # Handle string values
                elif isinstance(item, StringConfig):
                    item.value = args.value
                
                # Prepare updates dictionary with the required prefix
                prefix = ""
                writer_func = None
                
                if "app_modules_config.h" in config_file.filepath:
                    prefix = "modules"
                    writer_func = write_modules_config
                elif "csp_app_params.h" in config_file.filepath:
                    prefix = "params"
                    writer_func = write_params_config
                elif "csp_library_config.h" in config_file.filepath:
                    prefix = "library"
                    writer_func = write_library_config
                elif "ksdef.h" in config_file.filepath:
                    prefix = "ksdef"
                    writer_func = write_ksdef_config
                
                if writer_func:
                    # The writers expect boolean for toggles, int for numbers, str for strings
                    updates = {f"{prefix}_{item.name}": item.value}
                    from unified_config_manager.core import save_file
                    
                    updated_content = writer_func(config_file, updates)
                    save_file(config_file.filepath, updated_content)
                    config_file.original_content = updated_content
                else:
                    print(f"Error: No writer function found for {config_file.filepath}")
                    sys.exit(1)
                
                print(f"Successfully updated {item.name}")
                print_item(item)
                
            except ValueError:
                print(f"Error: Invalid numeric value for {item.name}: {args.value}")
                sys.exit(1)
            except Exception as e:
                print(f"Error saving changes: {e}")
                sys.exit(1)
        else:
            print(f"Error: Config '{args.name}' not found.")
            sys.exit(1)

    elif args.command == "export":
        from pathlib import Path
        dest_path = Path(args.dest)
        fw_src_path = Path(fw_src) if fw_src else None
        try:
            file_count, category_counts = export_all_configs(dest_path, framework_src=fw_src_path)
            print(f"Successfully exported {file_count} files to {dest_path}")
            for category, count in category_counts.items():
                print(f"  - {category}: {count} files")
        except Exception as e:
            print(f"Error: Export failed: {e}")
            sys.exit(1)
    else:
        parser.print_help()

if __name__ == "__main__":
    main()
