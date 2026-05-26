"""
Unified Configuration Manager Core Library

Core functionality for parsing, writing, and exporting embedded system configurations.
Extracted from unified_config_manager_v2.py to provide reusable, non-GUI components.
"""

from .models import (
    ConfigItem,
    ConfigType,
    ToggleConfig,
    NumberConfig,
    StringConfig,
    ConfigSection,
    ConfigFile,
)

from .config_io import (
    load_file,
    save_file,
)

from .config_parser import (
    parse_modules_config,
    parse_params_config,
    parse_library_config,
    parse_ksdef_config,
)

from .config_writer import (
    write_modules_config,
    write_params_config,
    write_library_config,
    write_ksdef_config,
)

from .export_manager import (
    export_tab,
    export_all_configs,
    get_export_files,
)

from .linker_updater import (
    update_linker_script,
)

from .project_updater import (
    update_cproject_file,
)

from .validator import (
    validate_file_exists,
    validate_config_content,
    validate_config_file,
    validate_export_path,
    validate_template_path,
    validate_config_update,
)

from .config_modifier import (
    add_config_to_file,
    delete_config_from_file,
    add_section_to_file,
)

__all__ = [
    # Models
    'ConfigItem',
    'ConfigType',
    'ToggleConfig',
    'NumberConfig',
    'StringConfig',
    'ConfigSection',
    'ConfigFile',
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
    'get_export_files',
    # Linker
    'update_linker_script',
    # Project
    'update_cproject_file',
    # Validator
    'validate_file_exists',
    'validate_config_content',
    'validate_config_file',
    'validate_export_path',
    'validate_template_path',
    'validate_config_update',
    # Config Modifier
    'add_config_to_file',
    'delete_config_from_file',
    'add_section_to_file',
]
