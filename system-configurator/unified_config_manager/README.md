# Unified Configuration Manager (UCM)

A modern tool for managing embedded system configurations with both GUI and CLI interfaces.

## 🚀 Features

- **Multi-Interface**: Modern PySide6 GUI with sidebar layout or Command-Line Interface (CLI)
- **Configuration Management**: Handle 4 types of configurations:
  - 📦 **Modules** (hardware/software enable/disable)
  - 🔧 **Parameters** (application settings)
  - 📚 **Library** (libcsp configurations)
  - ⚙️ **Kernel Services** (system definitions)
- **Auto-Detection**: Automatically locate configuration files
- **Validation**: Comprehensive configuration validation
- **Export**: Export configurations and related project folders to embedded projects
- **Real-time Updates**: Dirty-state tracking for unsaved changes (GUI)

## 📋 Requirements

- Python 3.8 or higher
- PySide6 (Required for GUI only)

## 🔧 Installation

```bash
pip install -r unified_config_manager/requirements.txt
```

Or install directly:

```bash
pip install PySide6
```

## 🎯 Quick Start

### Launch GUI

```bash
python unified_config_manager.py
```

Or use the batch file (Windows):
```bash
run_gui.bat
```

### Launch CLI

```bash
python unified_config_manager_cli.py --help
```

---

## 🖥️ Command-Line Interface (CLI) Usage

The CLI tool allows you to manage configurations directly from the terminal.

### 1. List configurations
```bash
# List all config names
python unified_config_manager_cli.py list

# List with current values and sections
python unified_config_manager_cli.py list --values --sections
```

### 2. Get a value
```bash
python unified_config_manager_cli.py get ENABLE_HW_CAN
```

### 3. Set a value
```bash
# Set toggle (supports 1/0, true/false, enable/disable)
python unified_config_manager_cli.py set ENABLE_HW_CAN 1

# Set numeric value
python unified_config_manager_cli.py set CSP_APP_CUSTOM_PORT_1 1234

# Set string value
python unified_config_manager_cli.py set CSP_DEFAULT_SERVER_NAME "MyServer"
```

### 4. Export all configurations
```bash
python unified_config_manager_cli.py export --dest ./exported_project
```

---

## 🎨 GUI Usage

### 1. Navigation
Use the sidebar to switch between Modules, Parameters, Library, and Kernel Services.

### 2. Editing
- Toggle checkboxes for enable/disable
- Adjust numbers with spin boxes
- Edit text in input fields
- Hover over items to see descriptions

### 3. Save & Export
- **Save**: Save current section changes to disk
- **Reset**: Revert unsaved changes
- **Export All**: Export all configurations to a project folder

---

## 📊 Configuration Types

| Type | File | Description | Count |
|------|------|-------------|-------|
| Modules | `app_modules_config.h` | Hardware/software modules | 19 |
| Parameters | `csp_app_params.h` | Application parameters | 18 |
| Library | `csp_library_config.h` | Library configurations | 81 |
| KS Def | `ksdef.h` | Kernel service definitions | 37 |

## 🏗️ Architecture

```
unified_config_manager/
├── core/              # Business logic (Parser, Writer, Models)
├── gui/               # PySide6 GUI interface
├── cli/               # Command-Line Interface
└── __init__.py
```

## 📝 Version

**Current Version:** 2.2.0 (GUI & CLI Support)

## 🤝 Contributing

This is a configuration management tool for embedded systems development at TSC Narit.

---

**Unified Configuration Manager** - Professional configuration management for embedded systems.
