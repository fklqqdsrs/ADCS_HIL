"""
Export manager for copying configurations to other projects.

Handles exporting individual tabs or all configurations including folders.
All operations preserve exact behavior from the original implementation.
"""

import shutil
from pathlib import Path
from typing import List, Tuple, Dict
from .config_io import FileOperationError


def get_export_files(tab_name: str) -> List[Tuple[str, str]]:
    """
    Get files to export for a specific tab.
    
    Args:
        tab_name: Name of the tab ('modules', 'params', 'library', 'ksdef')
        
    Returns:
        List of tuples (source_path, destination_path)
        
    Note:
        File mappings are preserved exactly from original implementation
    """
    export_map = {
        'modules': [
            ('Configuration/app_modules_config.h', 'Configuration/app_modules_config.h')
        ],
        'params': [
            ('Configuration/csp_app_params.h', 'Configuration/csp_app_params.h')
        ],
        'library': [
            ('Configuration/csp_library_config.h', 'Configuration/csp_library_config.h')
        ],
        'ksdef': [
            ('Configuration/ksdef.h', 'Configuration/ksdef.h')
        ]
    }
    
    return export_map.get(tab_name, [])


def export_tab(tab_name: str, export_path: Path) -> int:
    """
    Export specific tab configuration files.
    
    Args:
        tab_name: Name of the tab to export
        export_path: Destination directory path
        
    Returns:
        Number of files copied
        
    Raises:
        FileOperationError: If export fails
    """
    export_files = get_export_files(tab_name)
    
    copied_count = 0
    for src, dest in export_files:
        src_path = Path(src)
        if src_path.exists():
            dest_path = export_path / dest
            dest_path.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(src, dest_path)
            copied_count += 1
    
    return copied_count


def get_framework_path() -> Path:
    """
    Get the base path for the framework, supporting both new and legacy names.
    
    Returns:
        Path to the framework applications directory
    """
    # 1. Check inside framework folder (old submodule style)
    new_path = Path('framework/ThaiSpace-AstraCore-STM32')
    if new_path.exists():
        return new_path
        
    # 2. Check legacy applications folder
    legacy_path = Path('framework/applications')
    if legacy_path.exists():
        return legacy_path
        
    # 3. Check sibling directory (modern decoupled style)
    sibling_path = Path('../ThaiSpace-AstraCore-STM32')
    if sibling_path.exists():
        return sibling_path
        
    return new_path


def export_all_configs(export_path: Path, framework_src: Path = None) -> Tuple[int, Dict[str, int]]:
    """
    Export all configurations - copy entire Applications, Configuration and Core files.
    
    This function exports:
    - Entire Applications folder
    - Entire Configuration folder
    - 11 specific Core files (7 headers + 4 sources)
    - Drivers/s25fl128l folder
    - Middlewares/libcsp-1.6 folder
    - Middlewares/littlefs folder
    
    Args:
        export_path: Destination directory path (e.g., the STM32L496 folder)
        framework_src: Optional path to the source framework root. 
                      If not provided, it will be automatically detected.
        
    Returns:
        Tuple of (total_file_count, category_counts)
        
    Raises:
        FileOperationError: If export fails
    """
    category_counts = {}
    copied_count = 0
    
    fw_base = framework_src if framework_src else get_framework_path()
    
    # Copy entire Applications folder
    apps_src = fw_base / 'Applications'
    if apps_src.exists():
        apps_dest = export_path / 'Applications'
        if apps_dest.exists():
            shutil.rmtree(apps_dest)
        shutil.copytree(apps_src, apps_dest)
        app_count = sum(1 for _ in apps_src.rglob('*') if _.is_file())
        category_counts['Applications'] = app_count
        copied_count += app_count
    else:
        print(f"Warning: Applications source not found at {apps_src}")
    
    # Copy entire Configuration folder
    # Use script location to find Configuration folder safely
    tool_root = Path(__file__).parent.parent.parent
    config_src = tool_root / 'Configuration'
    
    if config_src.exists():
        config_dest = export_path / 'Configuration'
        if config_dest.exists():
            shutil.rmtree(config_dest)
        shutil.copytree(config_src, config_dest)
        config_count = sum(1 for _ in config_src.rglob('*') if _.is_file())
        category_counts['Configuration'] = config_count
        copied_count += config_count
    
    # Copy Core files (11 specific files: 7 headers + 4 sources)
    # Only copy these files, do NOT delete existing folders
    core_export_files = [
        (str(fw_base / 'Core/Inc/crc32.h'), 'Core/Inc/crc32.h'),
        (str(fw_base / 'Core/Inc/finsh.h'), 'Core/Inc/finsh.h'),
        (str(fw_base / 'Core/Inc/kservice.h'), 'Core/Inc/kservice.h'),
        (str(fw_base / 'Core/Inc/list_service.h'), 'Core/Inc/list_service.h'),
        (str(fw_base / 'Core/Inc/msh.h'), 'Core/Inc/msh.h'),
        (str(fw_base / 'Core/Inc/shell.h'), 'Core/Inc/shell.h'),
        (str(fw_base / 'Core/Inc/tsc_csp_app.h'), 'Core/Inc/tsc_csp_app.h'),
        (str(fw_base / 'Core/Src/kservice.c'), 'Core/Src/kservice.c'),
        (str(fw_base / 'Core/Src/msh.c'), 'Core/Src/msh.c'),
        (str(fw_base / 'Core/Src/shell.c'), 'Core/Src/shell.c'),
        (str(fw_base / 'Core/Src/tsc_csp_app.c'), 'Core/Src/tsc_csp_app.c')
    ]
    
    core_count = 0
    for src, dest in core_export_files:
        if Path(src).exists():
            dest_path = export_path / dest
            dest_path.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(src, dest_path)
            core_count += 1
    category_counts['Core'] = core_count
    copied_count += core_count
    
    # Copy Drivers/s25fl128l folder
    s25fl128l_src = fw_base / 'Drivers' / 's25fl128l'
    if s25fl128l_src.exists():
        s25fl128l_dest = export_path / 'Drivers' / 's25fl128l'
        if s25fl128l_dest.exists():
            shutil.rmtree(s25fl128l_dest)
        s25fl128l_dest.parent.mkdir(parents=True, exist_ok=True)
        shutil.copytree(s25fl128l_src, s25fl128l_dest)
        s25fl128l_count = sum(1 for _ in s25fl128l_src.rglob('*') if _.is_file())
        category_counts['Drivers/s25fl128l'] = s25fl128l_count
        copied_count += s25fl128l_count
    
    # Copy Middlewares/libcsp-1.6 folder
    libcsp_src = fw_base / 'Middlewares' / 'libcsp-1.6'
    if libcsp_src.exists():
        libcsp_dest = export_path / 'Middlewares' / 'libcsp-1.6'
        if libcsp_dest.exists():
            shutil.rmtree(libcsp_dest)
        libcsp_dest.parent.mkdir(parents=True, exist_ok=True)
        shutil.copytree(libcsp_src, libcsp_dest)
        libcsp_count = sum(1 for _ in libcsp_src.rglob('*') if _.is_file())
        category_counts['Middlewares/libcsp-1.6'] = libcsp_count
        copied_count += libcsp_count
    
    # Copy Middlewares/littlefs folder
    littlefs_src = fw_base / 'Middlewares' / 'littlefs'
    if littlefs_src.exists():
        littlefs_dest = export_path / 'Middlewares' / 'littlefs'
        if littlefs_dest.exists():
            shutil.rmtree(littlefs_dest)
        littlefs_dest.parent.mkdir(parents=True, exist_ok=True)
        shutil.copytree(littlefs_src, littlefs_dest)
        littlefs_count = sum(1 for _ in littlefs_src.rglob('*') if _.is_file())
        category_counts['Middlewares/littlefs'] = littlefs_count
        copied_count += littlefs_count
    
    return copied_count, category_counts
