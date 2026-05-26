#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
PySide6 GUI for Unified Configuration Manager

Modern, beautiful interface with enhanced UX.
All business logic is delegated to unified_config_manager.core modules.
"""

import sys
import logging
from pathlib import Path
from typing import Dict, List, Optional, Any
from copy import deepcopy
from PySide6.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout,
    QTabWidget, QLabel, QCheckBox, QSpinBox, QLineEdit, QPushButton,
    QScrollArea, QMessageBox, QFrame, QFileDialog, QStatusBar, QGraphicsDropShadowEffect,
    QDialog, QFormLayout, QComboBox, QTextEdit, QDialogButtonBox, QInputDialog
)
from PySide6.QtCore import Qt, QPropertyAnimation, QEasingCurve, Property, QTimer, Signal
from PySide6.QtGui import QColor, QShortcut, QKeySequence, QAction

# Setup logging with UTF-8 encoding to support emoji and Thai text
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
    handlers=[
        logging.FileHandler('unified_config_manager.log', encoding='utf-8'),
        logging.StreamHandler()
    ]
)
logger = logging.getLogger(__name__)

# Import core modules only - no business logic in GUI
from ..core import (
    ConfigType,
    parse_modules_config,
    parse_params_config,
    parse_library_config,
    parse_ksdef_config,
    write_modules_config,
    write_params_config,
    write_library_config,
    write_ksdef_config,
    save_file,
    load_file,
    export_all_configs,
    export_tab,
    update_linker_script,
    update_cproject_file,
    add_config_to_file,
    delete_config_from_file,
    add_section_to_file,
)

# Modern & Professional Color Palette
# Dark Theme
DARK_BACKGROUND_PRIMARY = "#0d1117"
DARK_BACKGROUND_SECONDARY = "#161b22"
DARK_CARD_BACKGROUND = "#21262d"
DARK_CARD_HOVER = "#2d333b"
DARK_TEXT_PRIMARY = "#f0f6fc"
DARK_TEXT_SECONDARY = "#8b949e"
DARK_TEXT_MUTED = "#6e7681"
DARK_BORDER_COLOR = "#30363d"

# Light Theme
LIGHT_BACKGROUND_PRIMARY = "#ffffff"
LIGHT_BACKGROUND_SECONDARY = "#f6f8fa"
LIGHT_CARD_BACKGROUND = "#ffffff"
LIGHT_CARD_HOVER = "#f6f8fa"
LIGHT_TEXT_PRIMARY = "#24292f"
LIGHT_TEXT_SECONDARY = "#57606a"
LIGHT_TEXT_MUTED = "#6e7781"
LIGHT_BORDER_COLOR = "#d0d7de"

# Accent Colors - Professional blue-purple
ACCENT = "#58a6ff"                  # Brighter, more visible blue
ACCENT_HOVER = "#79c0ff"            # Lighter on hover
ACCENT_PRESSED = "#388bfd"          # Darker when pressed
ACCENT_LIGHT = "#79c0ff"
ACCENT_GRADIENT_START = "#58a6ff"
ACCENT_GRADIENT_END = "#bc8cff"     # Purple gradient end

# Status Colors - More vibrant
SUCCESS = "#3fb950"                 # GitHub green
SUCCESS_LIGHT = "#56d364"
DANGER = "#f85149"                  # GitHub red
WARNING = "#d29922"                 # GitHub yellow
INFO = "#58a6ff"                    # GitHub blue

# Border & Effects - Subtle but visible
BORDER_LIGHT = "rgba(240, 246, 252, 0.1)"
GLOW_COLOR = "rgba(88, 166, 255, 0.4)"  # Blue glow


class ThemeManager:
    """Manages theme switching between dark and light modes"""
    
    def __init__(self):
        self.is_dark = True
        logger.info("ThemeManager initialized with dark theme")
    
    def toggle(self) -> bool:
        """Toggle between dark and light theme"""
        self.is_dark = not self.is_dark
        logger.info(f"Theme toggled to {'dark' if self.is_dark else 'light'}")
        return self.is_dark
    
    @property
    def BACKGROUND_PRIMARY(self) -> str:
        return DARK_BACKGROUND_PRIMARY if self.is_dark else LIGHT_BACKGROUND_PRIMARY
    
    @property
    def BACKGROUND_SECONDARY(self) -> str:
        return DARK_BACKGROUND_SECONDARY if self.is_dark else LIGHT_BACKGROUND_SECONDARY
    
    @property
    def CARD_BACKGROUND(self) -> str:
        return DARK_CARD_BACKGROUND if self.is_dark else LIGHT_CARD_BACKGROUND
    
    @property
    def CARD_HOVER(self) -> str:
        return DARK_CARD_HOVER if self.is_dark else LIGHT_CARD_HOVER
    
    @property
    def TEXT_PRIMARY(self) -> str:
        return DARK_TEXT_PRIMARY if self.is_dark else LIGHT_TEXT_PRIMARY
    
    @property
    def TEXT_SECONDARY(self) -> str:
        return DARK_TEXT_SECONDARY if self.is_dark else LIGHT_TEXT_SECONDARY
    
    @property
    def TEXT_MUTED(self) -> str:
        return DARK_TEXT_MUTED if self.is_dark else LIGHT_TEXT_MUTED
    
    @property
    def BORDER_COLOR(self) -> str:
        return DARK_BORDER_COLOR if self.is_dark else LIGHT_BORDER_COLOR


class UndoRedoManager:
    """Manages undo/redo functionality for configuration changes"""
    
    def __init__(self):
        self.undo_stack: List[Dict[str, Any]] = []
        self.redo_stack: List[Dict[str, Any]] = []
        self.max_stack_size = 50
        self.current_state: Optional[Dict[str, Any]] = None
        logger.info("UndoRedoManager initialized")
    
    def capture_state(self, section_name: str, config_values: Dict[str, Any]) -> None:
        """Capture current state before making changes"""
        if self.current_state is not None:
            # Push previous state to undo stack
            self.undo_stack.append(deepcopy(self.current_state))
            if len(self.undo_stack) > self.max_stack_size:
                self.undo_stack.pop(0)
            self.redo_stack.clear()  # Clear redo stack on new action
            logger.debug(f"State captured for {section_name}. Undo stack size: {len(self.undo_stack)}")
        
        # Store current state
        self.current_state = {
            'section': section_name,
            'values': deepcopy(config_values)
        }
    
    def push_state(self, state: Dict[str, Any]) -> None:
        """Push a new state to undo stack (legacy method)"""
        self.undo_stack.append(deepcopy(state))
        if len(self.undo_stack) > self.max_stack_size:
            self.undo_stack.pop(0)
        self.redo_stack.clear()  # Clear redo stack on new action
        logger.debug(f"State pushed to undo stack. Stack size: {len(self.undo_stack)}")
    
    def undo(self) -> Optional[Dict[str, Any]]:
        """Undo last action"""
        if not self.undo_stack:
            logger.warning("Undo stack is empty")
            return None
        
        # Push current state to redo stack
        if self.current_state:
            self.redo_stack.append(deepcopy(self.current_state))
        
        # Pop from undo stack
        state = self.undo_stack.pop()
        self.current_state = state
        logger.info(f"Undo performed. Undo stack: {len(self.undo_stack)}, Redo stack: {len(self.redo_stack)}")
        return state
    
    def redo(self) -> Optional[Dict[str, Any]]:
        """Redo last undone action"""
        if not self.redo_stack:
            logger.warning("Redo stack is empty")
            return None
        
        # Push current state to undo stack
        if self.current_state:
            self.undo_stack.append(deepcopy(self.current_state))
        
        # Pop from redo stack
        state = self.redo_stack.pop()
        self.current_state = state
        logger.info(f"Redo performed. Undo stack: {len(self.undo_stack)}, Redo stack: {len(self.redo_stack)}")
        return state
    
    def can_undo(self) -> bool:
        return len(self.undo_stack) > 0
    
    def can_redo(self) -> bool:
        return len(self.redo_stack) > 0
    
    def clear(self) -> None:
        """Clear both stacks"""
        self.undo_stack.clear()
        self.redo_stack.clear()
        self.current_state = None
        logger.info("Undo/Redo stacks cleared")


class ValidationRule:
    """Base class for validation rules"""
    
    def __init__(self, error_message: str):
        self.error_message = error_message
    
    def validate(self, value: Any) -> tuple[bool, str]:
        """Validate value. Returns (is_valid, error_message)"""
        raise NotImplementedError


class RangeValidation(ValidationRule):
    """Validates that number is within range"""
    
    def __init__(self, min_val: Optional[int] = None, max_val: Optional[int] = None):
        self.min_val = min_val
        self.max_val = max_val
        
        if min_val is not None and max_val is not None:
            msg = f"Value must be between {min_val} and {max_val}"
        elif min_val is not None:
            msg = f"Value must be at least {min_val}"
        elif max_val is not None:
            msg = f"Value must be at most {max_val}"
        else:
            msg = "Invalid range"
        
        super().__init__(msg)
    
    def validate(self, value: Any) -> tuple[bool, str]:
        try:
            num_val = int(value)
            if self.min_val is not None and num_val < self.min_val:
                return False, self.error_message
            if self.max_val is not None and num_val > self.max_val:
                return False, self.error_message
            return True, ""
        except (ValueError, TypeError):
            return False, "Value must be a number"


class PatternValidation(ValidationRule):
    """Validates string against regex pattern"""
    
    def __init__(self, pattern: str, error_message: str):
        import re
        self.pattern = re.compile(pattern)
        super().__init__(error_message)
    
    def validate(self, value: Any) -> tuple[bool, str]:
        str_val = str(value)
        if self.pattern.match(str_val):
            return True, ""
        return False, self.error_message


class NotEmptyValidation(ValidationRule):
    """Validates that string is not empty"""
    
    def __init__(self):
        super().__init__("Value cannot be empty")
    
    def validate(self, value: Any) -> tuple[bool, str]:
        str_val = str(value).strip()
        if str_val:
            return True, ""
        return False, self.error_message


class ConfigValidator:
    """Manages validation rules for configurations"""
    
    def __init__(self):
        self.rules: Dict[str, List[ValidationRule]] = {}
        logger.info("ConfigValidator initialized")
    
    def add_rule(self, config_key: str, rule: ValidationRule) -> None:
        """Add validation rule for a config"""
        if config_key not in self.rules:
            self.rules[config_key] = []
        self.rules[config_key].append(rule)
        logger.debug(f"Validation rule added for {config_key}")
    
    def validate(self, config_key: str, value: Any) -> tuple[bool, str]:
        """Validate a value. Returns (is_valid, error_message)"""
        if config_key not in self.rules:
            return True, ""  # No rules = valid
        
        for rule in self.rules[config_key]:
            is_valid, error_msg = rule.validate(value)
            if not is_valid:
                logger.debug(f"Validation failed for {config_key}: {error_msg}")
                return False, error_msg
        
        return True, ""
    
    def setup_default_rules(self) -> None:
        """Setup default validation rules for common configs"""
        # Example rules - customize based on your configs
        
        # Stack sizes should be positive and reasonable
        self.add_rule("modules_STACK_SIZE", RangeValidation(min_val=128, max_val=65536))
        self.add_rule("params_STACK_SIZE", RangeValidation(min_val=128, max_val=65536))
        
        # Priorities should be in valid range
        self.add_rule("modules_PRIORITY", RangeValidation(min_val=0, max_val=255))
        self.add_rule("params_PRIORITY", RangeValidation(min_val=0, max_val=255))
        
        # Queue sizes should be positive
        self.add_rule("modules_QUEUE_SIZE", RangeValidation(min_val=1, max_val=1000))
        self.add_rule("params_QUEUE_SIZE", RangeValidation(min_val=1, max_val=1000))
        
        logger.info("Default validation rules setup completed")


class AddConfigDialog(QDialog):
    """Dialog for adding new configuration"""
    
    def __init__(self, parent=None, theme: 'ThemeManager' = None, section_name: str = ""):
        super().__init__(parent)
        self.theme = theme
        self.section_name = section_name
        self.setWindowTitle(f"Add New Config to {section_name}")
        self.setMinimumWidth(500)
        
        layout = QVBoxLayout(self)
        layout.setSpacing(16)
        layout.setContentsMargins(20, 20, 20, 20)
        
        # Form layout
        form = QFormLayout()
        form.setSpacing(12)
        
        # Config name
        self.name_input = QLineEdit()
        self.name_input.setPlaceholderText("e.g., ENABLE_MY_FEATURE")
        form.addRow("Config Name:", self.name_input)
        
        # Description
        self.desc_input = QLineEdit()
        self.desc_input.setPlaceholderText("Brief description")
        form.addRow("Description:", self.desc_input)
        
        # Type
        self.type_combo = QComboBox()
        self.type_combo.addItems(["Toggle (0/1)", "Number", "String"])
        self.type_combo.currentIndexChanged.connect(self._on_type_changed)
        form.addRow("Type:", self.type_combo)
        
        # Default value
        self.value_input = QLineEdit()
        self.value_input.setText("1")
        self.value_input.setPlaceholderText("0 or 1")
        form.addRow("Default Value:", self.value_input)
        
        # Comment (optional)
        self.comment_input = QTextEdit()
        self.comment_input.setPlaceholderText("Optional detailed comment")
        self.comment_input.setMaximumHeight(80)
        form.addRow("Comment:", self.comment_input)
        
        layout.addLayout(form)
        
        # Buttons
        buttons = QDialogButtonBox(
            QDialogButtonBox.StandardButton.Ok | QDialogButtonBox.StandardButton.Cancel
        )
        buttons.accepted.connect(self.accept)
        buttons.rejected.connect(self.reject)
        layout.addWidget(buttons)
        
        self._apply_style()
    
    def _on_type_changed(self, index):
        """Update value input based on type"""
        if index == 0:  # Toggle
            self.value_input.setText("1")
            self.value_input.setPlaceholderText("0 or 1")
        elif index == 1:  # Number
            self.value_input.setText("0")
            self.value_input.setPlaceholderText("Any number")
        else:  # String
            self.value_input.setText("")
            self.value_input.setPlaceholderText("Any text")
    
    def _apply_style(self):
        """Apply theme styling"""
        if not self.theme:
            return
            
        self.setStyleSheet(f"""
            QDialog {{
                background-color: {self.theme.BACKGROUND_SECONDARY};
            }}
            QLabel {{
                color: {self.theme.TEXT_PRIMARY};
                font-size: 11px;
                font-weight: 500;
            }}
            QLineEdit, QTextEdit, QComboBox {{
                background-color: {self.theme.BACKGROUND_PRIMARY};
                color: {self.theme.TEXT_PRIMARY};
                border: 1.5px solid {self.theme.BORDER_COLOR};
                border-radius: 4px;
                padding: 6px;
                font-size: 11px;
            }}
            QLineEdit:focus, QTextEdit:focus, QComboBox:focus {{
                border-color: {ACCENT};
            }}
            QPushButton {{
                background-color: {ACCENT};
                color: white;
                font-weight: 600;
                font-size: 11px;
                padding: 8px 16px;
                border: none;
                border-radius: 4px;
            }}
            QPushButton:hover {{
                background-color: {ACCENT_HOVER};
            }}
        """)
    
    def get_config_data(self):
        """Get configuration data from dialog"""
        type_map = {0: ConfigType.TOGGLE, 1: ConfigType.NUMBER, 2: ConfigType.STRING}
        return {
            "name": self.name_input.text().strip(),
            "description": self.desc_input.text().strip(),
            "type": type_map[self.type_combo.currentIndex()],
            "value": self.value_input.text().strip(),
            "comment": self.comment_input.toPlainText().strip()
        }


class AddSectionDialog(QDialog):
    """Dialog for adding new section"""
    
    def __init__(self, parent=None, theme: 'ThemeManager' = None):
        super().__init__(parent)
        self.theme = theme
        self.setWindowTitle("Add New Section")
        self.setMinimumWidth(400)
        
        layout = QVBoxLayout(self)
        layout.setSpacing(16)
        layout.setContentsMargins(20, 20, 20, 20)
        
        # Form layout
        form = QFormLayout()
        form.setSpacing(12)
        
        # Section name
        self.name_input = QLineEdit()
        self.name_input.setPlaceholderText("e.g., MY NEW MODULES")
        form.addRow("Section Name:", self.name_input)
        
        # Description
        self.desc_input = QTextEdit()
        self.desc_input.setPlaceholderText("Optional description")
        self.desc_input.setMaximumHeight(80)
        form.addRow("Description:", self.desc_input)
        
        layout.addLayout(form)
        
        # Buttons
        buttons = QDialogButtonBox(
            QDialogButtonBox.StandardButton.Ok | QDialogButtonBox.StandardButton.Cancel
        )
        buttons.accepted.connect(self.accept)
        buttons.rejected.connect(self.reject)
        layout.addWidget(buttons)
        
        self._apply_style()
    
    def _apply_style(self):
        """Apply theme styling"""
        if not self.theme:
            return
            
        self.setStyleSheet(f"""
            QDialog {{
                background-color: {self.theme.BACKGROUND_SECONDARY};
            }}
            QLabel {{
                color: {self.theme.TEXT_PRIMARY};
                font-size: 11px;
                font-weight: 500;
            }}
            QLineEdit, QTextEdit {{
                background-color: {self.theme.BACKGROUND_PRIMARY};
                color: {self.theme.TEXT_PRIMARY};
                border: 1.5px solid {self.theme.BORDER_COLOR};
                border-radius: 4px;
                padding: 6px;
                font-size: 11px;
            }}
            QLineEdit:focus, QTextEdit:focus {{
                border-color: {ACCENT};
            }}
            QPushButton {{
                background-color: {ACCENT};
                color: white;
                font-weight: 600;
                font-size: 11px;
                padding: 8px 16px;
                border: none;
                border-radius: 4px;
            }}
            QPushButton:hover {{
                background-color: {ACCENT_HOVER};
            }}
        """)
    
    def get_section_data(self):
        """Get section data from dialog"""
        return {
            "name": self.name_input.text().strip(),
            "description": self.desc_input.toPlainText().strip()
        }


class ConfigItemWidget:
    """Base widget for configuration items"""
    
    def __init__(self, config, parent_section, theme: ThemeManager):
        self.config = config
        self.parent_section = parent_section
        self.theme = theme
        self.widget = None
    
    def get_value(self):
        """Get current value from widget"""
        raise NotImplementedError
    
    def set_value(self, value):
        """Set value in widget"""
        raise NotImplementedError


class ToggleItemWidget(ConfigItemWidget):
    """Modern checkbox widget - compact"""
    
    def __init__(self, config, parent_section, theme: ThemeManager, change_callback=None):
        super().__init__(config, parent_section, theme)
        self.widget = QCheckBox(config.name)
        self.widget.setChecked(bool(config.value))
        
        if change_callback:
            self.widget.stateChanged.connect(change_callback)
        
        self.widget.setToolTip(f"{config.description}\n\nCurrent: {'ON' if config.value else 'OFF'}")
        
        # Compact styling
        self.widget.setStyleSheet(f"""
            QCheckBox {{
                color: {self.theme.TEXT_PRIMARY};
                font-size: 11px;
                font-weight: 500;
                padding: 6px 10px;
                spacing: 10px;
                border-radius: 6px;
                background-color: transparent;
            }}
            QCheckBox:hover {{
                background-color: {self.theme.CARD_HOVER};
            }}
            QCheckBox::indicator {{
                width: 16px;
                height: 16px;
                border-radius: 4px;
                border: 1.5px solid {self.theme.BORDER_COLOR};
                background-color: {self.theme.BACKGROUND_SECONDARY};
            }}
            QCheckBox::indicator:hover {{
                border-color: {ACCENT};
                background-color: rgba(88, 166, 255, 0.1);
            }}
            QCheckBox::indicator:checked {{
                background-color: {ACCENT};
                border-color: {ACCENT};
                image: url(data:image/svg+xml;base64,PHN2ZyB3aWR0aD0iMTIiIGhlaWdodD0iMTIiIHZpZXdCb3g9IjAgMCAxMiAxMiIgZmlsbD0ibm9uZSIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIj4KPHBhdGggZD0iTTEwIDNMNC41IDguNUwyIDYiIHN0cm9rZT0id2hpdGUiIHN0cm9rZS13aWR0aD0iMS41IiBzdHJva2UtbGluZWNhcD0icm91bmQiIHN0cm9rZS1saW5lam9pbj0icm91bmQiLz4KPC9zdmc+Cg==);
            }}
            QCheckBox::indicator:checked:hover {{
                background-color: {ACCENT_HOVER};
            }}
        """)
    
    def get_value(self):
        return self.widget.isChecked()
    
    def set_value(self, value):
        self.widget.setChecked(bool(value))


class NumberItemWidget(ConfigItemWidget):
    """Modern number input - compact with validation"""
    
    def __init__(self, config, parent_section, theme: ThemeManager, change_callback=None, validator: Optional['ConfigValidator'] = None):
        super().__init__(config, parent_section, theme)
        self.validator = validator
        self.config_key = f"{parent_section.config_type_prefix}_{config.name}"
        
        self.widget = QSpinBox()
        self.widget.setRange(-2147483648, 2147483647)
        self.widget.setValue(int(config.value))
        self.widget.setMinimumWidth(110)
        self.widget.setFixedHeight(28)
        
        # Error label for validation
        self.error_label = QLabel()
        self.error_label.setStyleSheet(f"""
            QLabel {{
                color: {DANGER};
                font-size: 9px;
                font-weight: 500;
                padding: 2px 0;
            }}
        """)
        self.error_label.setVisible(False)
        
        if change_callback:
            self.widget.valueChanged.connect(lambda: self._on_change(change_callback))
        
        self.widget.setToolTip(f"{config.description}\n\nCurrent: {config.value}")
        
        # Compact styling
        self._update_style(True)
    
    def _on_change(self, callback):
        """Handle value change with validation"""
        if self.validator:
            value = self.widget.value()
            is_valid, error_msg = self.validator.validate(self.config_key, value)
            
            if not is_valid:
                self.error_label.setText(f"⚠️ {error_msg}")
                self.error_label.setVisible(True)
                self._update_style(False)
                logger.warning(f"Validation failed for {self.config_key}: {error_msg}")
            else:
                self.error_label.setVisible(False)
                self._update_style(True)
        
        callback()
    
    def _update_style(self, is_valid: bool):
        """Update widget style based on validation"""
        border_color = self.theme.BORDER_COLOR if is_valid else DANGER
        bg_color = self.theme.BACKGROUND_SECONDARY if is_valid else "rgba(248, 81, 73, 0.1)"
        
        self.widget.setStyleSheet(f"""
            QSpinBox {{
                background-color: {bg_color};
                color: {self.theme.TEXT_PRIMARY};
                border: 1.5px solid {border_color};
                border-radius: 4px;
                padding: 4px 8px;
                font-size: 11px;
                font-weight: 500;
            }}
            QSpinBox:hover {{
                border-color: {ACCENT if is_valid else DANGER};
                background-color: {self.theme.CARD_BACKGROUND};
            }}
            QSpinBox:focus {{
                border-color: {ACCENT if is_valid else DANGER};
                background-color: {self.theme.CARD_BACKGROUND};
            }}
            QSpinBox::up-button, QSpinBox::down-button {{
                background-color: {self.theme.CARD_BACKGROUND};
                border: none;
                width: 18px;
                border-radius: 3px;
                margin: 2px;
            }}
            QSpinBox::up-button:hover, QSpinBox::down-button:hover {{
                background-color: {ACCENT};
            }}
            QSpinBox::up-button:pressed, QSpinBox::down-button:pressed {{
                background-color: {ACCENT_PRESSED};
            }}
        """)
    
    def get_value(self):
        return self.widget.value()
    
    def set_value(self, value):
        self.widget.setValue(int(value))
        if self.validator:
            is_valid, _ = self.validator.validate(self.config_key, value)
            self._update_style(is_valid)


class StringItemWidget(ConfigItemWidget):
    """Modern text input - compact with validation"""
    
    def __init__(self, config, parent_section, theme: ThemeManager, change_callback=None, validator: Optional['ConfigValidator'] = None):
        super().__init__(config, parent_section, theme)
        self.validator = validator
        self.config_key = f"{parent_section.config_type_prefix}_{config.name}"
        
        self.widget = QLineEdit()
        self.widget.setText(str(config.value))
        self.widget.setMinimumWidth(180)
        self.widget.setFixedHeight(28)
        
        # Error label for validation
        self.error_label = QLabel()
        self.error_label.setStyleSheet(f"""
            QLabel {{
                color: {DANGER};
                font-size: 9px;
                font-weight: 500;
                padding: 2px 0;
            }}
        """)
        self.error_label.setVisible(False)
        
        if change_callback:
            self.widget.textChanged.connect(lambda: self._on_change(change_callback))
        
        self.widget.setToolTip(f"{config.description}\n\nCurrent: {config.value}")
        
        # Compact styling
        self._update_style(True)
    
    def _on_change(self, callback):
        """Handle value change with validation"""
        if self.validator:
            value = self.widget.text()
            is_valid, error_msg = self.validator.validate(self.config_key, value)
            
            if not is_valid:
                self.error_label.setText(f"⚠️ {error_msg}")
                self.error_label.setVisible(True)
                self._update_style(False)
                logger.warning(f"Validation failed for {self.config_key}: {error_msg}")
            else:
                self.error_label.setVisible(False)
                self._update_style(True)
        
        callback()
    
    def _update_style(self, is_valid: bool):
        """Update widget style based on validation"""
        border_color = self.theme.BORDER_COLOR if is_valid else DANGER
        bg_color = self.theme.BACKGROUND_SECONDARY if is_valid else "rgba(248, 81, 73, 0.1)"
        
        self.widget.setStyleSheet(f"""
            QLineEdit {{
                background-color: {bg_color};
                color: {self.theme.TEXT_PRIMARY};
                border: 1.5px solid {border_color};
                border-radius: 4px;
                padding: 4px 8px;
                font-size: 11px;
                font-weight: 500;
            }}
            QLineEdit:hover {{
                border-color: {ACCENT if is_valid else DANGER};
                background-color: {self.theme.CARD_BACKGROUND};
            }}
            QLineEdit:focus {{
                border-color: {ACCENT if is_valid else DANGER};
                background-color: {self.theme.CARD_BACKGROUND};
            }}
        """)
    
    def get_value(self):
        return self.widget.text()
    
    def set_value(self, value):
        self.widget.setText(str(value))
        if self.validator:
            is_valid, _ = self.validator.validate(self.config_key, value)
            self._update_style(is_valid)


class SectionCard:
    """Modern card widget - compact with validation support and action buttons"""
    
    def __init__(self, section, config_type_prefix, theme: ThemeManager, change_callback=None, validator: Optional[ConfigValidator] = None, parent_tab=None):
        self.section = section
        self.config_type_prefix = config_type_prefix
        self.theme = theme
        self.validator = validator
        self.config_widgets = {}  # Maps config_name -> ConfigItemWidget
        self.change_callback = change_callback  # Callback for value changes
        self.parent_tab = parent_tab  # Reference to parent TabContent
        
        # Compact card
        self.card = QFrame()
        self.card.setStyleSheet(f"""
            QFrame {{
                background-color: {self.theme.CARD_BACKGROUND};
                border-radius: 6px;
                border: 1px solid {self.theme.BORDER_COLOR};
                padding: 12px;
                margin: 6px 0;
            }}
            QFrame:hover {{
                border-color: {ACCENT};
                background-color: {self.theme.CARD_HOVER};
            }}
        """)
        
        # Subtle shadow
        shadow = QGraphicsDropShadowEffect()
        shadow.setBlurRadius(12)
        shadow.setXOffset(0)
        shadow.setYOffset(2)
        shadow.setColor(QColor(0, 0, 0, 40))
        self.card.setGraphicsEffect(shadow)
        
        layout = QVBoxLayout(self.card)
        layout.setSpacing(10)
        layout.setContentsMargins(0, 0, 0, 0)
        
        # Compact section header with action buttons
        header_layout = QHBoxLayout()
        header_layout.setSpacing(10)
        
        # Smaller icon
        icon_label = QLabel("📋")
        icon_label.setStyleSheet(f"""
            QLabel {{
                font-size: 14px;
                padding: 0;
            }}
        """)
        header_layout.addWidget(icon_label)
        
        # Compact title
        title_label = QLabel(section.name)
        title_label.setStyleSheet(f"""
            QLabel {{
                color: {self.theme.TEXT_PRIMARY};
                font-size: 12px;
                font-weight: 600;
                padding: 0;
            }}
        """)
        header_layout.addWidget(title_label, 1)
        
        # Compact count badge
        self.count_badge = QLabel(f"{len(section.configs)}")
        self.count_badge.setStyleSheet(f"""
            QLabel {{
                background-color: {ACCENT};
                color: white;
                font-size: 10px;
                font-weight: 600;
                padding: 2px 8px;
                border-radius: 8px;
            }}
        """)
        self.count_badge.setAlignment(Qt.AlignmentFlag.AlignCenter)
        header_layout.addWidget(self.count_badge)
        
        # Action buttons
        add_btn = QPushButton("➕")
        add_btn.setFixedSize(24, 24)
        add_btn.setCursor(Qt.CursorShape.PointingHandCursor)
        add_btn.setToolTip("Add new config to this section")
        add_btn.setStyleSheet(f"""
            QPushButton {{
                background-color: {SUCCESS};
                color: white;
                font-size: 12px;
                border: none;
                border-radius: 4px;
            }}
            QPushButton:hover {{
                background-color: {SUCCESS_LIGHT};
            }}
        """)
        add_btn.clicked.connect(self._on_add_config)
        header_layout.addWidget(add_btn)
        
        layout.addLayout(header_layout)
        
        # Compact divider
        divider = QFrame()
        divider.setFrameShape(QFrame.Shape.HLine)
        divider.setFrameShadow(QFrame.Shadow.Plain)
        divider.setFixedHeight(1)
        divider.setStyleSheet(f"""
            QFrame {{
                background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                    stop:0 transparent, 
                    stop:0.5 {self.theme.BORDER_COLOR}, 
                    stop:1 transparent);
                border: none;
            }}
        """)
        layout.addWidget(divider)
        
        # Store main layout for adding configs later
        self.main_layout = layout
        
        # Add configuration items
        for config in section.configs:
            item_widget = self._create_item_widget(config)
            self.config_widgets[config.name] = item_widget
            
            # Create container for widget and error label
            widget_container = QVBoxLayout()
            widget_container.setSpacing(2)
            widget_container.setContentsMargins(0, 0, 0, 0)
            
            # Compact spacing for items
            item_layout = QHBoxLayout()
            item_layout.setContentsMargins(6, 4, 6, 4)
            
            if config.config_type == ConfigType.TOGGLE:
                item_layout.addWidget(item_widget.widget, 1)
            else:
                desc_label = QLabel(config.description)
                desc_label.setStyleSheet(f"""
                    QLabel {{
                        color: {self.theme.TEXT_SECONDARY};
                        font-size: 11px;
                        font-weight: 500;
                        line-height: 1.4;
                    }}
                """)
                item_layout.addWidget(desc_label, 1)
                item_layout.addWidget(item_widget.widget, 0, Qt.AlignmentFlag.AlignRight)
            
            # Add action buttons for each config
            action_buttons_layout = QHBoxLayout()
            action_buttons_layout.setSpacing(4)
            
            # Delete button
            del_btn = QPushButton("🗑️")
            del_btn.setFixedSize(20, 20)
            del_btn.setCursor(Qt.CursorShape.PointingHandCursor)
            del_btn.setToolTip("Delete")
            del_btn.setStyleSheet(f"""
                QPushButton {{
                    background-color: transparent;
                    color: {self.theme.TEXT_SECONDARY};
                    font-size: 10px;
                    border: 1px solid {self.theme.BORDER_COLOR};
                    border-radius: 3px;
                }}
                QPushButton:hover {{
                    background-color: {DANGER};
                    color: white;
                    border-color: {DANGER};
                }}
            """)
            del_btn.clicked.connect(lambda checked, name=config.name: self._on_delete_config(name))
            action_buttons_layout.addWidget(del_btn)
            
            item_layout.addLayout(action_buttons_layout)
            
            widget_container.addLayout(item_layout)
            
            # Add error label if widget has one
            if hasattr(item_widget, 'error_label'):
                error_container = QHBoxLayout()
                error_container.setContentsMargins(6, 0, 6, 4)
                error_container.addStretch()
                error_container.addWidget(item_widget.error_label)
                widget_container.addLayout(error_container)
            
            layout.addLayout(widget_container)
    
    def _create_item_widget(self, config):
        """Create appropriate widget based on config type"""
        if config.config_type == ConfigType.TOGGLE:
            return ToggleItemWidget(config, self, self.theme, self.change_callback)
        elif config.config_type == ConfigType.NUMBER:
            return NumberItemWidget(config, self, self.theme, self.change_callback, self.validator)
        elif config.config_type == ConfigType.STRING:
            return StringItemWidget(config, self, self.theme, self.change_callback, self.validator)
        else:
            raise ValueError(f"Unknown config type: {config.config_type}")
    
    def get_updates(self):
        """Get all updates from widgets"""
        updates = {}
        for name, widget in self.config_widgets.items():
            key = f"{self.config_type_prefix}_{name}"
            updates[key] = widget.get_value()
        return updates
    
    def reset_values(self):
        """Reset all widgets to original values"""
        for name, widget in self.config_widgets.items():
            widget.set_value(widget.config.value)
    
    def _on_add_config(self):
        """เปิด dialog เพื่อเพิ่ม config ใหม่"""
        dialog = AddConfigDialog(self.card, self.theme, self.section.name)
        if dialog.exec() == QDialog.DialogCode.Accepted:
            data = dialog.get_config_data()
            
            # Validate input
            if not data['name']:
                QMessageBox.warning(self.card, "Invalid Input", "Config name cannot be empty")
                return
            
            if not data['value']:
                QMessageBox.warning(self.card, "Invalid Input", "Default value cannot be empty")
                return
            
            try:
                # Get filepath from parent tab
                if not self.parent_tab:
                    QMessageBox.warning(self.card, "Error", "Cannot determine config file")
                    return
                
                filepath = self.parent_tab.config_file.filepath
                
                # Add config to file
                add_config_to_file(
                    filepath=filepath,
                    section_name=self.section.name,
                    config_name=data['name'],
                    config_value=data['value'],
                    config_type=data['type'],
                    description=data['description'],
                    comment=data['comment']
                )
                
                logger.info(f"Added config {data['name']} to {self.section.name}")
                
                # Reload parent tab
                if self.parent_tab:
                    self.parent_tab.reload()
                
                # Show success message
                QMessageBox.information(
                    self.card,
                    "Success",
                    f"Config '{data['name']}' added successfully!"
                )
                
            except Exception as e:
                logger.error(f"Failed to add config: {str(e)}", exc_info=True)
                QMessageBox.critical(
                    self.card,
                    "Error",
                    f"Failed to add config:\n{str(e)}"
                )
    
    def _on_delete_config(self, config_name: str):
        """ลบ config"""
        reply = QMessageBox.question(
            self.card,
            'Confirm Delete',
            f'Are you sure you want to delete config "{config_name}"?\n\nThis action cannot be undone.',
            QMessageBox.StandardButton.Yes | QMessageBox.StandardButton.No,
            QMessageBox.StandardButton.No
        )
        
        if reply == QMessageBox.StandardButton.Yes:
            try:
                if not self.parent_tab:
                    QMessageBox.warning(self.card, "Error", "Cannot determine config file")
                    return
                
                filepath = self.parent_tab.config_file.filepath
                
                # Delete config from file
                delete_config_from_file(filepath, config_name)
                
                logger.info(f"Deleted config {config_name} from {self.section.name}")
                
                # Reload parent tab
                if self.parent_tab:
                    self.parent_tab.reload()
                
            except Exception as e:
                logger.error(f"Failed to delete config: {str(e)}", exc_info=True)
                QMessageBox.critical(
                    self.card,
                    "Error",
                    f"Failed to delete config:\n{str(e)}"
                )


class TabContent:
    """Content for a single configuration tab with validation"""
    
    def __init__(self, config_file, config_type_prefix, theme: ThemeManager, change_callback=None, validator: Optional[ConfigValidator] = None):
        self.config_file = config_file
        self.config_type_prefix = config_type_prefix
        self.theme = theme
        self.validator = validator
        self.section_cards = []  # List of SectionCard
        self.change_callback = change_callback  # Callback for value changes
        
        # Create scroll area
        self.scroll_area = QScrollArea()
        self.scroll_area.setWidgetResizable(True)
        self.scroll_area.setHorizontalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)
        self.scroll_area.setStyleSheet(f"""
            QScrollArea {{
                background-color: {self.theme.BACKGROUND_SECONDARY};
                border: none;
            }}
        """)
        
        # Create content widget - compact margins
        self.content_widget = QWidget()
        self.content_layout = QVBoxLayout(self.content_widget)
        self.content_layout.setSpacing(0)
        self.content_layout.setContentsMargins(14, 14, 14, 14)
        
        # Add sections
        for section in config_file.sections:
            section_card = SectionCard(section, config_type_prefix, theme, change_callback, validator, parent_tab=self)
            self.section_cards.append(section_card)
            self.content_layout.addWidget(section_card.card)
        
        # Add stretch at bottom
        self.content_layout.addStretch()
        
        self.scroll_area.setWidget(self.content_widget)
    
    def get_updates(self):
        """Get all updates from all sections"""
        updates = {}
        for section_card in self.section_cards:
            updates.update(section_card.get_updates())
        return updates
    
    def reset_values(self):
        """Reset all sections to original values"""
        for section_card in self.section_cards:
            section_card.reset_values()
    
    def reload(self):
        """Reload content after changes"""
        try:
            # Re-parse config file based on type
            if self.config_type_prefix == 'modules':
                self.config_file = parse_modules_config()
            elif self.config_type_prefix == 'params':
                self.config_file = parse_params_config()
            elif self.config_type_prefix == 'library':
                self.config_file = parse_library_config()
            elif self.config_type_prefix == 'ksdef':
                self.config_file = parse_ksdef_config()
            
            # Clear current widgets
            while self.content_layout.count():
                item = self.content_layout.takeAt(0)
                if item.widget():
                    item.widget().deleteLater()
            
            # Recreate section cards
            self.section_cards.clear()
            for section in self.config_file.sections:
                section_card = SectionCard(
                    section, 
                    self.config_type_prefix, 
                    self.theme, 
                    self.change_callback, 
                    self.validator,
                    parent_tab=self
                )
                self.section_cards.append(section_card)
                self.content_layout.addWidget(section_card.card)
            
            # Add stretch at bottom
            self.content_layout.addStretch()
            
            logger.info(f"Reloaded {self.config_type_prefix} configuration")
            
        except Exception as e:
            logger.error(f"Failed to reload configuration: {str(e)}", exc_info=True)
            QMessageBox.critical(
                None,
                "Reload Error",
                f"Failed to reload configuration:\n{str(e)}"
            )


class UnifiedConfigManagerPySide6(QMainWindow):
    """Main application window with modern sidebar layout and advanced features"""
    
    def __init__(self):
        super().__init__()
        self.setWindowTitle("TSC Narit - Unified Configuration Manager")
        self.resize(1600, 900)
        
        logger.info("Initializing Unified Configuration Manager")
        
        # Theme manager
        self.theme = ThemeManager()
        
        # Undo/Redo manager
        self.undo_redo = UndoRedoManager()
        
        # Validator
        self.validator = ConfigValidator()
        self.validator.setup_default_rules()
        
        # Store configuration files
        self.config_files: Dict[str, Any] = {}
        self.tab_contents: Dict[str, 'TabContent'] = {}
        self.unsaved_changes: Dict[str, bool] = {}
        self.save_button: Optional[QPushButton] = None
        self.export_all_button: Optional[QPushButton] = None
        self.status_bar: Optional[QFrame] = None
        self.current_section: str = 'modules'  # Track current section
        self.search_input: Optional[QLineEdit] = None
        
        # Set up UI
        self._setup_ui()
        
        # Setup keyboard shortcuts
        self._setup_shortcuts()
        
        # Apply dark theme
        self._apply_theme()
        
        # Load configurations
        self._load_all_configs()
        
        # Update status bar periodically
        self.status_timer = QTimer()
        self.status_timer.timeout.connect(self._update_status_bar)
        self.status_timer.start(1000)
        
        logger.info("Unified Configuration Manager initialized successfully")
    
    def _setup_shortcuts(self) -> None:
        """Setup keyboard shortcuts"""
        # Save shortcut (Ctrl+S)
        save_shortcut = QShortcut(QKeySequence.StandardKey.Save, self)
        save_shortcut.activated.connect(self._save_current_section)
        logger.info("Keyboard shortcut registered: Ctrl+S for Save")
        
        # Reset shortcut (Ctrl+R)
        reset_shortcut = QShortcut(QKeySequence("Ctrl+R"), self)
        reset_shortcut.activated.connect(self._reset_current_section_with_confirm)
        logger.info("Keyboard shortcut registered: Ctrl+R for Reset")
        
        # Undo shortcut (Ctrl+Z)
        undo_shortcut = QShortcut(QKeySequence.StandardKey.Undo, self)
        undo_shortcut.activated.connect(self._undo_action)
        logger.info("Keyboard shortcut registered: Ctrl+Z for Undo")
        
        # Redo shortcut (Ctrl+Y or Ctrl+Shift+Z)
        redo_shortcut = QShortcut(QKeySequence.StandardKey.Redo, self)
        redo_shortcut.activated.connect(self._redo_action)
        logger.info("Keyboard shortcut registered: Ctrl+Y for Redo")
        
        # Export All shortcut (Ctrl+E)
        export_shortcut = QShortcut(QKeySequence("Ctrl+E"), self)
        export_shortcut.activated.connect(self._export_all_configs)
        logger.info("Keyboard shortcut registered: Ctrl+E for Export All")
        
        # Search shortcut (Ctrl+F)
        search_shortcut = QShortcut(QKeySequence.StandardKey.Find, self)
        search_shortcut.activated.connect(self._focus_search)
        logger.info("Keyboard shortcut registered: Ctrl+F for Search")
        
        # Theme toggle (Ctrl+T)
        theme_shortcut = QShortcut(QKeySequence("Ctrl+T"), self)
        theme_shortcut.activated.connect(self._toggle_theme)
        logger.info("Keyboard shortcut registered: Ctrl+T for Theme Toggle")
    
    def _setup_ui(self):
        """Set up the main UI with sidebar layout"""
        # Central widget
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        main_layout = QHBoxLayout(central_widget)
        main_layout.setContentsMargins(0, 0, 0, 0)
        main_layout.setSpacing(0)
        
        # 1. Sidebar (Left)
        sidebar = self._create_sidebar()
        main_layout.addWidget(sidebar)
        
        # 2. Main Content Area (Right)
        content_area = QWidget()
        content_layout = QVBoxLayout(content_area)
        content_layout.setContentsMargins(0, 0, 0, 0)
        content_layout.setSpacing(0)
        
        # Header with actions
        header = self._create_content_header()
        content_layout.addWidget(header)
        
        # Content stack (for switching between sections)
        self.content_stack = QWidget()
        self.content_stack_layout = QVBoxLayout(self.content_stack)
        self.content_stack_layout.setContentsMargins(0, 0, 0, 0)
        content_layout.addWidget(self.content_stack, 1)
        
        # Status Bar
        self.status_bar = self._create_status_bar()
        content_layout.addWidget(self.status_bar)
        
        main_layout.addWidget(content_area, 1)
        
        # Create content containers
        self._create_content_containers()
    
    def _create_sidebar(self):
        """Create compact sidebar navigation"""
        sidebar = QFrame()
        sidebar.setFixedWidth(200)
        sidebar.setStyleSheet(f"""
            QFrame {{
                background-color: {self.theme.BACKGROUND_SECONDARY};
                border-right: 1px solid {self.theme.BORDER_COLOR};
            }}
        """)
        
        layout = QVBoxLayout(sidebar)
        layout.setContentsMargins(0, 0, 0, 0)
        layout.setSpacing(0)
        
        # Compact Logo/Title Section
        logo_section = QWidget()
        logo_section.setFixedHeight(60)
        logo_section.setStyleSheet(f"""
            QWidget {{
                background-color: {self.theme.BACKGROUND_PRIMARY};
                border-bottom: 1px solid {self.theme.BORDER_COLOR};
            }}
        """)
        logo_layout = QVBoxLayout(logo_section)
        logo_layout.setContentsMargins(16, 12, 16, 12)
        logo_layout.setSpacing(2)
        
        title = QLabel("Configuration")
        title.setStyleSheet(f"""
            QLabel {{
                color: {self.theme.TEXT_PRIMARY};
                font-size: 13px;
                font-weight: 700;
            }}
        """)
        logo_layout.addWidget(title)
        
        subtitle = QLabel("Manager")
        subtitle.setStyleSheet(f"""
            QLabel {{
                color: {self.theme.TEXT_SECONDARY};
                font-size: 10px;
                font-weight: 500;
            }}
        """)
        logo_layout.addWidget(subtitle)
        
        layout.addWidget(logo_section)
        
        # Navigation Items - compact
        nav_container = QWidget()
        nav_layout = QVBoxLayout(nav_container)
        nav_layout.setContentsMargins(10, 14, 10, 14)
        nav_layout.setSpacing(6)
        
        # Navigation buttons
        self.nav_buttons = {}
        nav_items = [
            ('modules', '📦', 'Modules'),
            ('params', '🔧', 'Parameters'),
            ('library', '📚', 'Library'),
            ('ksdef', '⚙️', 'Kernel Services')
        ]
        
        for key, icon, label in nav_items:
            btn = self._create_nav_button(key, icon, label)
            self.nav_buttons[key] = btn
            nav_layout.addWidget(btn)
        
        nav_layout.addStretch()
        layout.addWidget(nav_container, 1)
        
        # Compact version info at bottom
        version_section = QWidget()
        version_section.setFixedHeight(36)
        version_section.setStyleSheet(f"""
            QWidget {{
                background-color: {self.theme.BACKGROUND_PRIMARY};
                border-top: 1px solid {self.theme.BORDER_COLOR};
            }}
        """)
        version_layout = QHBoxLayout(version_section)
        version_layout.setContentsMargins(16, 0, 16, 0)
        
        version_label = QLabel("v2.1")
        version_label.setStyleSheet(f"""
            QLabel {{
                color: {self.theme.TEXT_MUTED};
                font-size: 9px;
                font-weight: 500;
            }}
        """)
        version_layout.addWidget(version_label)
        version_layout.addStretch()
        
        layout.addWidget(version_section)
        
        # Set initial active button
        self.nav_buttons['modules'].setProperty('active', True)
        self.nav_buttons['modules'].style().unpolish(self.nav_buttons['modules'])
        self.nav_buttons['modules'].style().polish(self.nav_buttons['modules'])
        
        return sidebar
    
    def _create_nav_button(self, key, icon, label):
        """Create a compact navigation button"""
        btn = QPushButton(f"{icon}  {label}")
        btn.setFixedHeight(34)
        btn.setCursor(Qt.CursorShape.PointingHandCursor)
        btn.setProperty('active', False)
        btn.setStyleSheet(f"""
            QPushButton {{
                background-color: transparent;
                color: {self.theme.TEXT_SECONDARY};
                font-size: 11px;
                font-weight: 500;
                text-align: left;
                padding-left: 14px;
                border: none;
                border-radius: 6px;
            }}
            QPushButton:hover {{
                background-color: {self.theme.CARD_BACKGROUND};
                color: {self.theme.TEXT_PRIMARY};
            }}
            QPushButton[active="true"] {{
                background-color: {ACCENT};
                color: white;
                font-weight: 600;
            }}
            QPushButton[active="true"]:hover {{
                background-color: {ACCENT_HOVER};
            }}
        """)
        btn.clicked.connect(lambda: self._switch_section(key))
        return btn
    
    def _create_content_header(self):
        """Create compact header for main content area with search"""
        header = QFrame()
        header.setFixedHeight(56)
        header.setStyleSheet(f"""
            QFrame {{
                background-color: {self.theme.BACKGROUND_SECONDARY};
                border-bottom: 1px solid {self.theme.BORDER_COLOR};
            }}
        """)
        
        layout = QHBoxLayout(header)
        layout.setContentsMargins(20, 12, 20, 12)
        layout.setSpacing(14)
        
        # Compact section title
        self.section_title = QLabel("Modules Configuration")
        self.section_title.setStyleSheet(f"""
            QLabel {{
                color: {self.theme.TEXT_PRIMARY};
                font-size: 13px;
                font-weight: 700;
            }}
        """)
        layout.addWidget(self.section_title)
        
        # Search bar
        self.search_input = QLineEdit()
        self.search_input.setPlaceholderText("🔍 Search configs... (Ctrl+F)")
        self.search_input.setFixedWidth(250)
        self.search_input.setFixedHeight(30)
        self.search_input.textChanged.connect(self._filter_configs)
        self.search_input.setStyleSheet(f"""
            QLineEdit {{
                background-color: {self.theme.BACKGROUND_PRIMARY};
                color: {self.theme.TEXT_PRIMARY};
                border: 1.5px solid {self.theme.BORDER_COLOR};
                border-radius: 5px;
                padding: 4px 10px;
                font-size: 11px;
            }}
            QLineEdit:focus {{
                border-color: {ACCENT};
            }}
        """)
        layout.addWidget(self.search_input)
        
        layout.addStretch()
        
        # Add Section button
        add_section_btn = QPushButton("➕ Section")
        add_section_btn.setFixedHeight(30)
        add_section_btn.setCursor(Qt.CursorShape.PointingHandCursor)
        add_section_btn.setToolTip("Add new section to current file")
        add_section_btn.setStyleSheet(f"""
            QPushButton {{
                background-color: {SUCCESS};
                color: white;
                font-weight: 600;
                font-size: 11px;
                padding: 0 14px;
                border: none;
                border-radius: 5px;
            }}
            QPushButton:hover {{
                background-color: {SUCCESS_LIGHT};
            }}
        """)
        add_section_btn.clicked.connect(self._add_section)
        layout.addWidget(add_section_btn)
        
        # Theme toggle button
        theme_btn = QPushButton("🌓")
        theme_btn.setFixedSize(30, 30)
        theme_btn.setCursor(Qt.CursorShape.PointingHandCursor)
        theme_btn.setToolTip("Toggle Theme (Ctrl+T)")
        theme_btn.setStyleSheet(f"""
            QPushButton {{
                background-color: {self.theme.CARD_BACKGROUND};
                color: {self.theme.TEXT_PRIMARY};
                font-size: 14px;
                border: 1.5px solid {self.theme.BORDER_COLOR};
                border-radius: 5px;
            }}
            QPushButton:hover {{
                background-color: {ACCENT};
                border-color: {ACCENT};
            }}
        """)
        theme_btn.clicked.connect(self._toggle_theme)
        layout.addWidget(theme_btn)
        
        # Export Current Section button
        export_section_btn = QPushButton("Export Section")
        export_section_btn.setFixedHeight(30)
        export_section_btn.setCursor(Qt.CursorShape.PointingHandCursor)
        export_section_btn.setToolTip("Export current section only")
        export_section_btn.setStyleSheet(f"""
            QPushButton {{
                background-color: transparent;
                color: {self.theme.TEXT_SECONDARY};
                font-weight: 500;
                font-size: 11px;
                padding: 0 14px;
                border: 1.5px solid {self.theme.BORDER_COLOR};
                border-radius: 5px;
            }}
            QPushButton:hover {{
                color: {self.theme.TEXT_PRIMARY};
                border-color: {ACCENT};
                background-color: {self.theme.CARD_BACKGROUND};
            }}
        """)
        export_section_btn.clicked.connect(self._export_current_section)
        layout.addWidget(export_section_btn)
        
        # Compact action buttons
        # Export All
        export_all_btn = QPushButton("Export All")
        export_all_btn.setFixedHeight(30)
        export_all_btn.setCursor(Qt.CursorShape.PointingHandCursor)
        export_all_btn.setToolTip("Export all configurations (Ctrl+E)")
        export_all_btn.setStyleSheet(f"""
            QPushButton {{
                background-color: {ACCENT};
                color: white;
                font-weight: 600;
                font-size: 11px;
                padding: 0 18px;
                border: none;
                border-radius: 5px;
            }}
            QPushButton:hover {{
                background-color: {ACCENT_HOVER};
            }}
            QPushButton:pressed {{
                background-color: {ACCENT_PRESSED};
            }}
        """)
        export_all_btn.clicked.connect(self._export_all_configs)
        self.export_all_button = export_all_btn
        
        # Save
        save_btn = QPushButton("Save")
        save_btn.setFixedHeight(30)
        save_btn.setCursor(Qt.CursorShape.PointingHandCursor)
        save_btn.setToolTip("Save changes (Ctrl+S)")
        save_btn.setStyleSheet(f"""
            QPushButton {{
                background-color: transparent;
                color: {self.theme.TEXT_PRIMARY};
                font-weight: 600;
                font-size: 11px;
                padding: 0 18px;
                border: 1.5px solid {ACCENT};
                border-radius: 5px;
            }}
            QPushButton:hover {{
                background-color: {ACCENT};
                color: white;
            }}
            QPushButton:disabled {{
                border-color: {self.theme.BORDER_COLOR};
                color: {self.theme.TEXT_MUTED};
            }}
        """)
        save_btn.clicked.connect(self._save_current_section)
        self.save_button = save_btn
        
        # Reset
        reset_btn = QPushButton("Reset")
        reset_btn.setFixedHeight(30)
        reset_btn.setCursor(Qt.CursorShape.PointingHandCursor)
        reset_btn.setToolTip("Reset changes (Ctrl+R)")
        reset_btn.setStyleSheet(f"""
            QPushButton {{
                background-color: transparent;
                color: {self.theme.TEXT_SECONDARY};
                font-weight: 500;
                font-size: 11px;
                padding: 0 18px;
                border: 1.5px solid {self.theme.BORDER_COLOR};
                border-radius: 5px;
            }}
            QPushButton:hover {{
                color: {self.theme.TEXT_PRIMARY};
                border-color: {self.theme.TEXT_SECONDARY};
                background-color: {self.theme.CARD_BACKGROUND};
            }}
        """)
        reset_btn.clicked.connect(self._reset_current_section_with_confirm)
        
        layout.addWidget(export_all_btn)
        layout.addWidget(save_btn)
        layout.addWidget(reset_btn)
        
        return header
    
    def _create_content_containers(self):
        """Create content containers for each section"""
        sections = ['modules', 'params', 'library', 'ksdef']
        
        for section in sections:
            container = QWidget()
            container.setProperty('section', section)
            container.setVisible(section == 'modules')  # Only show modules initially
            self.content_stack_layout.addWidget(container)
    
    def _switch_section(self, section_key):
        """Switch to a different section"""
        # Update active button
        for key, btn in self.nav_buttons.items():
            btn.setProperty('active', key == section_key)
            btn.style().unpolish(btn)
            btn.style().polish(btn)
        
        # Update section title
        titles = {
            'modules': 'Modules Configuration',
            'params': 'Parameters Configuration',
            'library': 'Library Configuration',
            'ksdef': 'Kernel Services Configuration'
        }
        self.section_title.setText(titles.get(section_key, 'Configuration'))
        
        # Show/hide content
        for i in range(self.content_stack_layout.count()):
            widget = self.content_stack_layout.itemAt(i).widget()
            if widget:
                widget.setVisible(widget.property('section') == section_key)
        
        # Update current section
        self.current_section = section_key
        
        # Update save button state
        self._update_save_button_state()
    
    def _save_current_section(self):
        """Save current section (renamed from _save_current_tab)"""
        logger.info(f"Saving section: {self.current_section}")
        self._save_current_tab()  # Call existing method
    
    def _reset_current_section(self):
        """Reset current section (renamed from _reset_current_tab)"""
        logger.info(f"Resetting section: {self.current_section}")
        self._reset_current_tab()  # Call existing method
    
    def _reset_current_section_with_confirm(self) -> None:
        """Reset current section with confirmation dialog"""
        reply = QMessageBox.question(
            self,
            'Confirm Reset',
            f'Are you sure you want to reset all changes in {self.current_section.upper()}?\n\nThis action cannot be undone.',
            QMessageBox.StandardButton.Yes | QMessageBox.StandardButton.No,
            QMessageBox.StandardButton.No
        )
        
        if reply == QMessageBox.StandardButton.Yes:
            logger.info(f"User confirmed reset for section: {self.current_section}")
            self._reset_current_section()
        else:
            logger.info(f"User cancelled reset for section: {self.current_section}")
    
    def _undo_action(self) -> None:
        """Undo last action"""
        if not self.undo_redo.can_undo():
            logger.warning("Cannot undo: stack is empty")
            self.status_message.setText("⚠️ Nothing to undo")
            self.status_message.setStyleSheet(f"""
                QLabel {{
                    color: {WARNING};
                    font-size: 10px;
                    font-weight: 500;
                }}
            """)
            return
        
        state = self.undo_redo.undo()
        if state:
            section_name = state['section']
            values = state['values']
            
            # Restore values to widgets
            if section_name in self.tab_contents:
                tab_content = self.tab_contents[section_name]
                for section_card in tab_content.section_cards:
                    for config_name, widget in section_card.config_widgets.items():
                        key = f"{section_card.config_type_prefix}_{config_name}"
                        if key in values:
                            widget.set_value(values[key])
                
                logger.info(f"Undo performed for {section_name}")
                self.status_message.setText(f"↶ Undo: Restored previous state")
                self.status_message.setStyleSheet(f"""
                    QLabel {{
                        color: {INFO};
                        font-size: 10px;
                        font-weight: 500;
                    }}
                """)
    
    def _redo_action(self) -> None:
        """Redo last undone action"""
        if not self.undo_redo.can_redo():
            logger.warning("Cannot redo: stack is empty")
            self.status_message.setText("⚠️ Nothing to redo")
            self.status_message.setStyleSheet(f"""
                QLabel {{
                    color: {WARNING};
                    font-size: 10px;
                    font-weight: 500;
                }}
            """)
            return
        
        state = self.undo_redo.redo()
        if state:
            section_name = state['section']
            values = state['values']
            
            # Restore values to widgets
            if section_name in self.tab_contents:
                tab_content = self.tab_contents[section_name]
                for section_card in tab_content.section_cards:
                    for config_name, widget in section_card.config_widgets.items():
                        key = f"{section_card.config_type_prefix}_{config_name}"
                        if key in values:
                            widget.set_value(values[key])
                
                logger.info(f"Redo performed for {section_name}")
                self.status_message.setText(f"↷ Redo: Restored next state")
                self.status_message.setStyleSheet(f"""
                    QLabel {{
                        color: {INFO};
                        font-size: 10px;
                        font-weight: 500;
                    }}
                """)
    
    def _toggle_theme(self) -> None:
        """Toggle between dark and light theme"""
        self.theme.toggle()
        self._apply_theme()
        
        # Reload all tabs to apply new theme
        for section_name in self.tab_contents:
            try:
                self.tab_contents[section_name].reload()
            except Exception as e:
                logger.error(f"Failed to reload tab {section_name}: {e}")
        
        logger.info(f"Theme toggled to {'dark' if self.theme.is_dark else 'light'}")
        
        # Show notification
        theme_name = "Dark" if self.theme.is_dark else "Light"
        self.status_message.setText(f"🎨 Switched to {theme_name} theme")
    
    def _focus_search(self) -> None:
        """Focus on search input"""
        if self.search_input:
            self.search_input.setFocus()
            self.search_input.selectAll()
            logger.debug("Search input focused")
    
    def _filter_configs(self, search_text: str) -> None:
        """Filter configurations based on search text"""
        logger.debug(f"Filtering configs with search text: {search_text}")
        search_text = search_text.lower()
        
        # Get current tab content
        if self.current_section not in self.tab_contents:
            return
        
        tab_content = self.tab_contents[self.current_section]
        
        # Show/hide section cards based on search
        for section_card in tab_content.section_cards:
            # Check if section name or any config matches
            section_matches = search_text in section_card.section.name.lower()
            
            # Check if any config in section matches
            config_matches = any(
                search_text in config.name.lower() or 
                search_text in config.description.lower()
                for config in section_card.section.configs
            )
            
            # Show card if matches
            should_show = section_matches or config_matches or search_text == ""
            section_card.card.setVisible(should_show)
        
        logger.debug(f"Config filtering completed")
    
    def _export_current_section(self) -> None:
        """Export only current section"""
        section_name = self.current_section
        logger.info(f"Exporting section: {section_name}")
        
        # Ask user for destination directory
        export_path_str = QFileDialog.getExistingDirectory(
            self,
            f"Export {section_name.upper()} Configuration",
            str(Path.cwd()),
            QFileDialog.Option.ShowDirsOnly
        )
        
        if not export_path_str:
            logger.info("Export cancelled by user")
            return  # User cancelled
        
        export_path = Path(export_path_str)
        
        try:
            # Call core export_tab
            files_exported = export_tab(section_name, export_path)
            
            logger.info(f"Successfully exported {files_exported} files for {section_name}")
            
            # Show success message
            QMessageBox.information(
                self,
                "Export Complete",
                f"{section_name.upper()} exported successfully!\n\n"
                f"Export Path: {export_path}\n"
                f"Files Exported: {files_exported}"
            )
            
            # Update status
            self.status_message.setText(f"✅ {section_name.upper()} exported successfully")
            
        except Exception as e:
            logger.error(f"Failed to export {section_name}: {str(e)}", exc_info=True)
            QMessageBox.critical(
                self,
                "Export Error",
                f"Failed to export {section_name.upper()}:\n{str(e)}"
            )
    
    def _create_status_bar(self):
        """Create compact status bar"""
        status_bar = QFrame()
        status_bar.setFixedHeight(32)
        status_bar.setObjectName("status_bar")
        status_bar.setStyleSheet(f"""
            QFrame {{
                background-color: {self.theme.BACKGROUND_PRIMARY};
                border-top: 1px solid {self.theme.BORDER_COLOR};
            }}
        """)
        
        layout = QHBoxLayout(status_bar)
        layout.setContentsMargins(20, 6, 20, 6)
        layout.setSpacing(16)
        
        # Compact status message
        self.status_message = QLabel("✅ Ready")
        self.status_message.setStyleSheet(f"""
            QLabel {{
                color: {SUCCESS};
                font-size: 10px;
                font-weight: 600;
            }}
        """)
        layout.addWidget(self.status_message)
        
        layout.addStretch()
        
        # Compact config count
        self.config_count_label = QLabel("Configs: 0")
        self.config_count_label.setStyleSheet(f"""
            QLabel {{
                color: {self.theme.TEXT_SECONDARY};
                font-size: 10px;
                font-weight: 500;
            }}
        """)
        layout.addWidget(self.config_count_label)
        
        # Separator
        sep1 = QLabel("•")
        sep1.setStyleSheet(f"color: {self.theme.BORDER_COLOR}; font-size: 10px;")
        layout.addWidget(sep1)
        
        # Compact unsaved changes indicator
        self.unsaved_indicator = QLabel("No unsaved changes")
        self.unsaved_indicator.setStyleSheet(f"""
            QLabel {{
                color: {self.theme.TEXT_SECONDARY};
                font-size: 10px;
                font-weight: 500;
            }}
        """)
        layout.addWidget(self.unsaved_indicator)
        
        return status_bar
    
    def _update_status_bar(self):
        """Update status bar information - compact"""
        # Count total configs
        total_configs = sum(
            len(cf.get_all_configs()) 
            for cf in self.config_files.values()
        )
        self.config_count_label.setText(f"Configs: {total_configs}")
        
        # Check unsaved changes
        unsaved_count = sum(1 for v in self.unsaved_changes.values() if v)
        if unsaved_count > 0:
            self.unsaved_indicator.setText(f"⚠️ {unsaved_count} section(s) with unsaved changes")
            self.unsaved_indicator.setStyleSheet(f"""
                QLabel {{
                    color: {WARNING};
                    font-size: 10px;
                    font-weight: 600;
                }}
            """)
        else:
            self.unsaved_indicator.setText("✓ All changes saved")
            self.unsaved_indicator.setStyleSheet(f"""
                QLabel {{
                    color: {SUCCESS};
                    font-size: 10px;
                    font-weight: 500;
                }}
            """)
    
    def _apply_theme(self):
        """Apply theme (dark or light) dynamically"""
        logger.info(f"Applying {'dark' if self.theme.is_dark else 'light'} theme")
        
        self.setStyleSheet(f"""
            QMainWindow {{
                background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                    stop:0 {self.theme.BACKGROUND_PRIMARY}, 
                    stop:0.5 {self.theme.BACKGROUND_SECONDARY},
                    stop:1 {self.theme.BACKGROUND_PRIMARY});
            }}
            
            QTabWidget::pane {{
                background: {self.theme.BACKGROUND_SECONDARY};
                border: none;
            }}
            
            QTabBar::tab {{
                background-color: {self.theme.BACKGROUND_SECONDARY};
                color: {self.theme.TEXT_SECONDARY};
                padding: 10px 20px;
                font-weight: 500;
                font-size: 11px;
                border: 1px solid {self.theme.BORDER_COLOR};
                border-bottom: none;
                border-top-left-radius: 6px;
                border-top-right-radius: 6px;
                margin-right: 4px;
                margin-top: 4px;
            }}
            
            QTabBar::tab:hover {{
                color: {self.theme.TEXT_PRIMARY};
                border-color: {ACCENT};
            }}
            
            QTabBar::tab:selected {{
                background-color: {ACCENT};
                color: white;
                margin-top: 0px;
                font-weight: 600;
            }}
            
            QScrollBar:vertical {{
                background-color: {self.theme.CARD_BACKGROUND};
                width: 14px;
                border-radius: 7px;
                margin: 2px;
            }}
            
            QScrollBar::handle:vertical {{
                background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                    stop:0 {ACCENT_GRADIENT_START}, 
                    stop:1 {ACCENT_GRADIENT_END});
                border-radius: 7px;
                min-height: 40px;
                border: 1px solid {self.theme.BORDER_COLOR};
            }}
            
            QScrollBar::handle:vertical:hover {{
                background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                    stop:0 {ACCENT_HOVER}, 
                    stop:1 {ACCENT_LIGHT});
            }}
            
            QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {{
                height: 0px;
            }}
            
            QScrollBar:horizontal {{
                background-color: {self.theme.CARD_BACKGROUND};
                height: 14px;
                border-radius: 7px;
                margin: 2px;
            }}
            
            QScrollBar::handle:horizontal {{
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 {ACCENT_GRADIENT_START}, 
                    stop:1 {ACCENT_GRADIENT_END});
                border-radius: 7px;
                min-width: 40px;
                border: 1px solid {self.theme.BORDER_COLOR};
            }}
            
            QScrollBar::handle:horizontal:hover {{
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 {ACCENT_HOVER}, 
                    stop:1 {ACCENT_LIGHT});
            }}
            
            QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {{
                width: 0px;
            }}
            
            QMessageBox {{
                background-color: {self.theme.BACKGROUND_SECONDARY};
            }}
            
            QMessageBox QLabel {{
                color: {self.theme.TEXT_PRIMARY};
                font-size: 13px;
                font-weight: 500;
            }}
            
            QMessageBox QPushButton {{
                background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                    stop:0 {ACCENT_GRADIENT_START}, 
                    stop:1 {ACCENT_GRADIENT_END});
                color: white;
                font-weight: 700;
                padding: 10px 24px;
                border: 1.5px solid {self.theme.BORDER_COLOR};
                border-radius: 8px;
                min-width: 90px;
            }}
            
            QMessageBox QPushButton:hover {{
                background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                    stop:0 {ACCENT_HOVER}, 
                    stop:1 {ACCENT_LIGHT});
            }}
        """)
        
        # Refresh all widgets to apply new theme
        self._refresh_all_widgets()
    
    def _refresh_all_widgets(self) -> None:
        """Refresh all widgets to apply new theme"""
        logger.debug("Refreshing all widgets for theme change")
        # Simply reload the entire UI would be easier
        # For now, just update the status message
        if hasattr(self, 'status_message'):
            self.status_message.setText(f"🎨 Theme changed to {'Dark' if self.theme.is_dark else 'Light'}")
        logger.debug("Widget refresh completed")
    
    def _load_all_configs(self):
        """Load all configurations using core parsers"""
        try:
            # Load modules config
            self.config_files['modules'] = parse_modules_config()
            self.unsaved_changes['modules'] = False
            self._update_section_content('modules')
            
            # Load params config
            self.config_files['params'] = parse_params_config()
            self.unsaved_changes['params'] = False
            self._update_section_content('params')
            
            # Load library config
            self.config_files['library'] = parse_library_config()
            self.unsaved_changes['library'] = False
            self._update_section_content('library')
            
            # Load ksdef config
            self.config_files['ksdef'] = parse_ksdef_config()
            self.unsaved_changes['ksdef'] = False
            self._update_section_content('ksdef')
            
            # Update save button state
            self._update_save_button_state()
            
            # Update status bar
            self._update_status_bar()
            
            # Show success status
            self.status_message.setText("✅ All configurations loaded")
            self.status_message.setStyleSheet(f"""
                QLabel {{
                    color: {SUCCESS};
                    font-size: 10px;
                    font-weight: 500;
                }}
            """)
            
        except Exception as e:
            QMessageBox.critical(
                self,
                "Error",
                f"Failed to load configurations:\n{str(e)}"
            )
    
    def _update_section_content(self, section_name):
        """Update section content with loaded configuration"""
        config_file = self.config_files[section_name]
        
        # Create tab content with change callback, theme, and validator
        tab_content = TabContent(config_file, section_name, self.theme, self._on_value_changed, self.validator)
        self.tab_contents[section_name] = tab_content
        
        # Find section container and add content
        for i in range(self.content_stack_layout.count()):
            widget = self.content_stack_layout.itemAt(i).widget()
            if widget and widget.property('section') == section_name:
                # Clear existing layout
                if widget.layout():
                    QWidget().setLayout(widget.layout())
                
                # Add new content
                layout = QVBoxLayout(widget)
                layout.setContentsMargins(0, 0, 0, 0)
                layout.addWidget(tab_content.scroll_area)
                break
    
    def _update_save_button_state(self):
        """Update save button based on current section's unsaved changes"""
        if self.save_button:
            self.save_button.setEnabled(self.unsaved_changes.get(self.current_section, False))
    
    def _on_value_changed(self):
        """Handle value change from any widget with state capture"""
        # Capture current state for undo
        if self.current_section in self.tab_contents:
            current_values = self.tab_contents[self.current_section].get_updates()
            self.undo_redo.capture_state(self.current_section, current_values)
        
        # Mark current section as having unsaved changes
        self.unsaved_changes[self.current_section] = True
        # Update save button state
        self._update_save_button_state()
        # Update status bar
        self._update_status_bar()
        # Update status message
        self.status_message.setText(f"⚠️ Unsaved changes in {self.current_section.upper()}")
        self.status_message.setStyleSheet(f"""
            QLabel {{
                color: {WARNING};
                font-size: 10px;
                font-weight: 500;
            }}
        """)
    
    def _save_current_tab(self):
        """Save current section configuration using core writers"""
        tab_name = self.current_section
        
        # Check if tab content exists
        if tab_name not in self.tab_contents:
            QMessageBox.warning(self, "Warning", f"No configuration to save for {tab_name.upper()}")
            return
        
        try:
            # Get updates from widgets
            updates = self.tab_contents[tab_name].get_updates()
            
            # Get current config file
            config_file = self.config_files[tab_name]
            
            # Call appropriate core writer with config_file and updates
            if tab_name == 'modules':
                content = write_modules_config(config_file, updates)
            elif tab_name == 'params':
                content = write_params_config(config_file, updates)
            elif tab_name == 'library':
                content = write_library_config(config_file, updates)
            elif tab_name == 'ksdef':
                content = write_ksdef_config(config_file, updates)
            else:
                QMessageBox.warning(self, "Error", f"Unknown section: {tab_name}")
                return
            
            # Save to file using core I/O
            save_file(config_file.filepath, content)
            
            # Reload config to update stored values
            if tab_name == 'modules':
                config_file = parse_modules_config()
            elif tab_name == 'params':
                config_file = parse_params_config()
            elif tab_name == 'library':
                config_file = parse_library_config()
            elif tab_name == 'ksdef':
                config_file = parse_ksdef_config()
            
            # Update stored config_file
            self.config_files[tab_name] = config_file
            
            # Reload the section content to refresh UI
            self._update_section_content(tab_name)
            
            # Clear unsaved changes flag
            self.unsaved_changes[tab_name] = False
            self._update_save_button_state()
            
            # Update status bar
            self._update_status_bar()
            
            # Show success message
            self.status_message.setText(f"✅ {tab_name.upper()} saved successfully")
            self.status_message.setStyleSheet(f"""
                QLabel {{
                    color: {SUCCESS};
                    font-size: 10px;
                    font-weight: 500;
                }}
            """)
            
            QMessageBox.information(
                self,
                "Success",
                f"{tab_name.upper()} configuration saved successfully."
            )
            
        except Exception as e:
            QMessageBox.critical(
                self,
                "Error",
                f"Failed to save {tab_name.upper()} configuration:\n{str(e)}"
            )
    
    def _reset_current_tab(self):
        """Reset current section values"""
        tab_name = self.current_section
        if tab_name in self.tab_contents:
            self.tab_contents[tab_name].reset_values()
            # Clear unsaved changes flag
            self.unsaved_changes[tab_name] = False
            self._update_save_button_state()
            # Update status bar
            self._update_status_bar()
            # Update status message
            self.status_message.setText(f"🔄 {tab_name.upper()} reset to saved values")
            self.status_message.setStyleSheet(f"""
                QLabel {{
                    color: {INFO};
                    font-size: 10px;
                    font-weight: 500;
                }}
            """)
    
    def _export_all_configs(self):
        """Export all configurations using core export_manager"""
        try:
            # Ask user for destination directory
            export_path_str = QFileDialog.getExistingDirectory(
                self,
                "Select Export Destination",
                str(Path.cwd()),
                QFileDialog.Option.ShowDirsOnly
            )
            
            if not export_path_str:
                return  # User cancelled
            
            # Convert to Path object
            export_path = Path(export_path_str)
            
            # Call core export_all_configs
            file_count, category_counts = export_all_configs(export_path)
            
            # Show success message with details
            message = (
                f"Export completed successfully!\n\n"
                f"Export Path: {export_path}\n"
                f"Total Files: {file_count}\n"
            )
            
            # Update status message
            self.status_message.setText(f"✅ Exported {file_count} files successfully")
            self.status_message.setStyleSheet(f"""
                QLabel {{
                    color: {SUCCESS};
                    font-size: 10px;
                    font-weight: 500;
                }}
            """)
            
            QMessageBox.information(
                self,
                "Export Complete",
                message
            )
            
        except Exception as e:
            QMessageBox.critical(
                self,
                "Export Error",
                f"Failed to export configurations:\n{str(e)}"
            )
    
    def _add_section(self):
        """เพิ่ม section ใหม่"""
        dialog = AddSectionDialog(self, self.theme)
        if dialog.exec() == QDialog.DialogCode.Accepted:
            data = dialog.get_section_data()
            
            # Validate input
            if not data['name']:
                QMessageBox.warning(self, "Invalid Input", "Section name cannot be empty")
                return
            
            try:
                # Get current config file path
                if self.current_section not in self.config_files:
                    QMessageBox.warning(self, "Error", "Cannot determine config file")
                    return
                
                filepath = self.config_files[self.current_section].filepath
                
                # Add section to file
                add_section_to_file(
                    filepath=filepath,
                    section_name=data['name'],
                    description=data['description']
                )
                
                logger.info(f"Added section {data['name']} to {filepath}")
                
                # Reload current tab
                self._reload_current_tab()
                
                # Show success message
                QMessageBox.information(
                    self,
                    "Success",
                    f"Section '{data['name']}' added successfully!"
                )
                
            except Exception as e:
                logger.error(f"Failed to add section: {str(e)}", exc_info=True)
                QMessageBox.critical(
                    self,
                    "Error",
                    f"Failed to add section:\n{str(e)}"
                )
    
    def _reload_current_tab(self):
        """Reload current tab content"""
        if self.current_section in self.tab_contents:
            self.tab_contents[self.current_section].reload()


def main():
    """Main entry point"""
    app = QApplication(sys.argv)
    app.setStyle('Fusion')
    
    window = UnifiedConfigManagerPySide6()
    window.show()
    
    sys.exit(app.exec())


if __name__ == '__main__':
    main()
