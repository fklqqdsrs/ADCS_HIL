"""
Data models for configuration items and files.

These models represent the structure of configuration data parsed from C header files.
"""

from dataclasses import dataclass, field
from typing import List, Dict, Any, Optional, Union
from enum import Enum


class ConfigType(Enum):
    """Type of configuration item."""
    TOGGLE = "toggle"
    NUMBER = "number"
    STRING = "string"


@dataclass
class ConfigItem:
    """Base class for configuration items."""
    name: str
    description: str
    value: Union[int, bool, str]
    config_type: ConfigType
    
    def __repr__(self):
        return f"{self.name} = {self.value} ({self.config_type.value})"


@dataclass
class ToggleConfig(ConfigItem):
    """Boolean/toggle configuration (0 or 1)."""
    def __init__(self, name: str, description: str, value: int):
        super().__init__(
            name=name,
            description=description,
            value=bool(value),
            config_type=ConfigType.TOGGLE
        )
        self._int_value = value
    
    @property
    def int_value(self) -> int:
        """Get integer value (0 or 1)."""
        return 1 if self.value else 0


@dataclass
class NumberConfig(ConfigItem):
    """Numeric configuration (integer)."""
    def __init__(self, name: str, description: str, value: int):
        super().__init__(
            name=name,
            description=description,
            value=int(value),
            config_type=ConfigType.NUMBER
        )


@dataclass
class StringConfig(ConfigItem):
    """String configuration (quoted string)."""
    def __init__(self, name: str, description: str, value: str):
        # Strip quotes if present
        if value.startswith('"') and value.endswith('"'):
            value = value[1:-1]
        super().__init__(
            name=name,
            description=description,
            value=value,
            config_type=ConfigType.STRING
        )
    
    @property
    def quoted_value(self) -> str:
        """Get value with quotes."""
        return f'"{self.value}"'


@dataclass
class ConfigSection:
    """A section containing related configuration items."""
    name: str
    configs: List[ConfigItem] = field(default_factory=list)
    
    def add_config(self, config: ConfigItem):
        """Add a configuration item to this section."""
        self.configs.append(config)
    
    def get_config(self, name: str) -> Optional[ConfigItem]:
        """Get a configuration item by name."""
        for config in self.configs:
            if config.name == name:
                return config
        return None
    
    def __repr__(self):
        return f"Section: {self.name} ({len(self.configs)} items)"


@dataclass
class ConfigFile:
    """Complete configuration file with multiple sections."""
    filepath: str
    original_content: str = ""
    sections: List[ConfigSection] = field(default_factory=list)
    
    def add_section(self, section: ConfigSection):
        """Add a section to this config file."""
        self.sections.append(section)
    
    def get_section(self, name: str) -> Optional[ConfigSection]:
        """Get a section by name."""
        for section in self.sections:
            if section.name == name:
                return section
        return None
    
    def get_config(self, name: str) -> Optional[ConfigItem]:
        """Get a configuration item by name (searches all sections)."""
        for section in self.sections:
            config = section.get_config(name)
            if config:
                return config
        return None
    
    def get_all_configs(self) -> Dict[str, ConfigItem]:
        """Get all configurations as a dictionary."""
        result = {}
        for section in self.sections:
            for config in section.configs:
                result[config.name] = config
        return result
    
    def __repr__(self):
        return f"ConfigFile: {self.filepath} ({len(self.sections)} sections, {sum(len(s.configs) for s in self.sections)} items)"
