"""
File I/O operations for configuration files.

Provides functions to read and write configuration files with error handling.
"""

from pathlib import Path
from typing import Optional


class FileOperationError(Exception):
    """Exception raised when file operations fail."""
    pass


def load_file(filepath: str) -> Optional[str]:
    """
    Load file content.
    
    Args:
        filepath: Path to the file to load
        
    Returns:
        File content as string, or None if error occurs
        
    Raises:
        FileOperationError: If file cannot be loaded
    """
    try:
        path = Path(filepath)
        with open(path, 'r', encoding='utf-8') as f:
            return f.read()
    except FileNotFoundError as e:
        raise FileOperationError(f"File not found: {filepath}") from e
    except Exception as e:
        raise FileOperationError(f"Error loading file {filepath}: {str(e)}") from e


def save_file(filepath: str, content: str) -> bool:
    """
    Save content to file.
    
    Args:
        filepath: Path to the file to save
        content: Content to write to the file
        
    Returns:
        True if successful
        
    Raises:
        FileOperationError: If file cannot be saved
    """
    try:
        path = Path(filepath)
        # Ensure parent directory exists
        path.parent.mkdir(parents=True, exist_ok=True)
        
        with open(path, 'w', encoding='utf-8') as f:
            f.write(content)
        return True
    except Exception as e:
        raise FileOperationError(f"Error saving file {filepath}: {str(e)}") from e
