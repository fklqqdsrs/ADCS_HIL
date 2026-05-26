"""
Validation and safety checks for configuration operations.

Provides validation functions for files and configuration content.
All logic is preserved exactly from the original implementation's checks.
"""

from pathlib import Path
from typing import List, Optional, Tuple
from .models import ConfigFile, ConfigItem, ConfigType
from .config_io import FileOperationError


class ValidationError(Exception):
    """Exception raised when validation fails."""
    pass


def validate_file_exists(filepath: str) -> bool:
    """
    Validate that a file exists.
    
    Args:
        filepath: Path to the file to validate
        
    Returns:
        True if file exists
        
    Raises:
        ValidationError: If file does not exist
    """
    path = Path(filepath)
    if not path.exists():
        raise ValidationError(f"File does not exist: {filepath}")
    if not path.is_file():
        raise ValidationError(f"Path is not a file: {filepath}")
    return True


def validate_config_content(content: str) -> bool:
    """
    Validate configuration file content.
    
    Args:
        content: File content to validate
        
    Returns:
        True if content is valid
        
    Raises:
        ValidationError: If content is invalid
    """
    if not content:
        raise ValidationError("Configuration content is empty")
    return True


def validate_config_file(config_file: ConfigFile) -> List[str]:
    """
    Validate a complete ConfigFile object.
    
    Checks:
    - Original content exists
    - At least one section is present
    - Each section has configurations
    - Each configuration has valid name and description
    
    Args:
        config_file: ConfigFile object to validate
        
    Returns:
        List of validation errors (empty if valid)
        
    Raises:
        ValidationError: If critical validation fails
    """
    errors = []
    
    # Check original content
    if not config_file.original_content:
        errors.append("Original content is empty")
    
    # Check sections exist
    if not config_file.sections:
        errors.append("No configuration sections found")
    else:
        # Validate each section
        for section in config_file.sections:
            if not section.name:
                errors.append("Section has no name")
            
            if not section.configs:
                errors.append(f"Section '{section.name}' has no configurations")
            else:
                # Validate each configuration
                for config in section.configs:
                    if not config.name:
                        errors.append(f"Configuration in section '{section.name}' has no name")
                    if not config.description:
                        errors.append(f"Configuration '{config.name}' has no description")
                    
                    # Validate value based on type
                    if config.config_type == ConfigType.NUMBER:
                        if not isinstance(config.value, (int, float)):
                            errors.append(f"Configuration '{config.name}' has invalid number value")
                    elif config.config_type == ConfigType.STRING:
                        if not isinstance(config.value, str):
                            errors.append(f"Configuration '{config.name}' has invalid string value")
                    elif config.config_type == ConfigType.TOGGLE:
                        if not isinstance(config.value, bool) and config.value not in (0, 1):
                            errors.append(f"Configuration '{config.name}' has invalid toggle value")
    
    return errors


def validate_export_path(export_path: Path) -> bool:
    """
    Validate export path.
    
    Args:
        export_path: Path to validate
        
    Returns:
        True if path is valid
        
    Raises:
        ValidationError: If path is invalid
    """
    if not export_path:
        raise ValidationError("Export path is empty")
    
    if not export_path.is_absolute():
        raise ValidationError("Export path must be absolute")
    
    # Check if parent directory exists
    if not export_path.parent.exists():
        raise ValidationError(f"Parent directory does not exist: {export_path.parent}")
    
    # Check if export path exists (it's OK if it doesn't, we'll create it)
    if export_path.exists():
        if not export_path.is_dir():
            raise ValidationError(f"Export path exists but is not a directory: {export_path}")
    
    return True


def validate_template_path(template_path: Path) -> bool:
    """
    Validate template file path.
    
    Args:
        template_path: Path to template file to validate
        
    Returns:
        True if template is valid
        
    Raises:
        ValidationError: If template is invalid
    """
    if not template_path.exists():
        raise ValidationError(f"Template file does not exist: {template_path}")
    if not template_path.is_file():
        raise ValidationError(f"Template path is not a file: {template_path}")
    
    # Try to read template content
    try:
        content = template_path.read_text(encoding='utf-8')
        if not content:
            raise ValidationError(f"Template file is empty: {template_path}")
    except Exception as e:
        raise ValidationError(f"Cannot read template file: {template_path} - {str(e)}")
    
    return True


def validate_config_update(config_file: ConfigFile, updates: dict) -> List[str]:
    """
    Validate configuration updates.
    
    Args:
        config_file: ConfigFile object
        updates: Dictionary of updates to validate
        
    Returns:
        List of validation errors (empty if valid)
    """
    errors = []
    
    for key, value in updates.items():
        # Extract config name (remove prefix)
        if '_' in key:
            name = '_'.join(key.split('_')[1:])
        else:
            name = key
        
        # Check if config exists
        config = config_file.get_config(name)
        if not config:
            errors.append(f"Configuration '{name}' does not exist")
            continue
        
        # Validate value based on type
        if config.config_type == ConfigType.NUMBER:
            try:
                int(value)
            except (ValueError, TypeError):
                errors.append(f"Configuration '{name}' value is not a valid number: {value}")
        
        elif config.config_type == ConfigType.STRING:
            if not isinstance(value, str):
                errors.append(f"Configuration '{name}' value is not a string: {value}")
        
        elif config.config_type == ConfigType.TOGGLE:
            if value not in (0, 1, True, False):
                errors.append(f"Configuration '{name}' value is not a valid toggle (0/1): {value}")
    
    return errors
