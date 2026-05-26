"""
Unified Configuration Manager - GUI Only

A modern GUI tool for managing embedded system configurations.
Supports modules, parameters, library configs, and kernel service definitions.
"""

__version__ = '2.1.0'
__author__ = 'TSC Narit'

# Import only core modules needed by GUI
from .core import (
    # Models
    ConfigItem,
    ToggleConfig,
    NumberConfig,
    StringConfig,
    ConfigSection,
    ConfigFile,
    ConfigType,
    # I/O
    load_file,
    save_file,
    # Parser
    parse_modules_config,
    parse_params_config,
    parse_library_config,
    parse_ksdef_config,
    # Writer
    write_modules_config,
    write_params_config,
    write_library_config,
    write_ksdef_config,
    # Export
    export_tab,
    export_all_configs,
    # Linker & Project
    update_linker_script,
    update_cproject_file,
)

__all__ = [
    '__version__',
    '__author__',
    # Models
    'ConfigItem',
    'ToggleConfig',
    'NumberConfig',
    'StringConfig',
    'ConfigSection',
    'ConfigFile',
    'ConfigType',
    # I/O
    'load_file',
    'save_file',
    # Parser
    'parse_modules_config',
    'parse_params_config',
    'parse_library_config',
    'parse_ksdef_config',
    # Writer
    'write_modules_config',
    'write_params_config',
    'write_library_config',
    'write_ksdef_config',
    # Export
    'export_tab',
    'export_all_configs',
    # Linker & Project
    'update_linker_script',
    'update_cproject_file',
]
