"""
Linker script updater for adding FINSH shell sections.

Updates linker scripts (*.ld files) with FINSH shell sections.
All regex patterns and logic are preserved exactly from the original implementation.
"""

import re
from pathlib import Path
from typing import Tuple, Optional
from .config_io import FileOperationError


class LinkerUpdateError(Exception):
    """Exception raised when linker script update fails."""
    pass


def update_linker_script(export_path: Path) -> bool:
    """
    Update linker script with FINSH shell sections.
    
    Finds linker script (looks for *_FLASH.ld) and inserts FINSH shell sections
    after *(.eh_frame) and before KEEP (*(.init)).
    
    Args:
        export_path: Path to the project directory
        
    Returns:
        True if successful, False otherwise
        
    Raises:
        LinkerUpdateError: If linker script cannot be found or updated
    """
    # Find linker script (look for *_FLASH.ld)
    linker_scripts = list(export_path.glob('*_FLASH.ld'))
    
    if not linker_scripts:
        # Try to find in common locations
        for ld_file in export_path.rglob('*.ld'):
            if 'FLASH' in ld_file.name or 'flash' in ld_file.name:
                linker_scripts.append(ld_file)
    
    if not linker_scripts:
        raise LinkerUpdateError(
            f"Linker script not found at: {export_path}\n"
            "Looking for *_FLASH.ld files"
        )
    
    # Use first found linker script
    linker_path = linker_scripts[0]
    
    try:
        # Read linker script content
        with open(linker_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # Check if sections already exist
        if '__fsymtab_start' in content:
            # Sections already exist, no update needed
            return True
        
        # FINSH shell sections to insert (exact text from original)
        finsh_sections = '''
/* section information for finsh shell */
    . = ALIGN(4);
    __fsymtab_start = .;
    KEEP(*(FSymTab))
 	__fsymtab_end = .;

    . = ALIGN(4);
	__start_thread_table = .;
	KEEP(*(.thread_table))
	__stop_thread_table = .;
	
	    . = ALIGN(4);
	__start_config_table = .;
	KEEP(*(.config_table))
	__stop_config_table = .;

'''
        
        # Find insertion point: after *(.eh_frame) and before KEEP (*(.init))
        # Exact regex pattern from original: (\*\(\.eh_frame\))\s+(KEEP\s+\(\*\(\.init\)\))
        pattern = r'(\*\(\.eh_frame\))\s+(KEEP\s+\(\*\(\.init\)\))'
        replacement = r'\1\n' + finsh_sections + r'\2'
        
        updated_content = re.sub(pattern, replacement, content, flags=re.MULTILINE | re.DOTALL)
        
        if updated_content == content:
            # Alternative: insert after .eh_frame section (before KEEP)
            # Exact regex pattern from original: (\*\(\.eh_frame\))\s*(?=KEEP)
            pattern2 = r'(\*\(\.eh_frame\))\s*(?=KEEP)'
            replacement2 = r'\1\n' + finsh_sections
            updated_content = re.sub(pattern2, replacement2, content, flags=re.MULTILINE)
        
        if updated_content == content:
            raise LinkerUpdateError(
                f"Could not find suitable insertion point in linker script\n"
                f"File: {linker_path}\n"
                f"Looking for pattern: *(.eh_frame) followed by KEEP (*(.init))"
            )
        
        # Write updated content
        with open(linker_path, 'w', encoding='utf-8') as f:
            f.write(updated_content)
        
        return True
        
    except LinkerUpdateError:
        raise
    except Exception as e:
        raise LinkerUpdateError(
            f"Error updating linker script {linker_path}: {str(e)}"
        ) from e
