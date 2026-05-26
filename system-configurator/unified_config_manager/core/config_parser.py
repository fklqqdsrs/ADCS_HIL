"""
Configuration file parsers for C header files.

Parsers for modules, params, library, and ksdef configuration files.
All regex patterns are preserved exactly from the original implementation.
"""

import re
from typing import List, Dict, Tuple, Union
from .models import ConfigFile, ConfigSection, ToggleConfig, NumberConfig, StringConfig
from .config_io import load_file, FileOperationError


def _parse_configs_in_order(content: str, known_configs: Dict[str, Tuple[str, str]]) -> List[Tuple[str, str, int]]:
    """
    Parse configs in the order they appear in the file.
    
    Args:
        content: File content
        known_configs: Dict of {config_name: (description, type)}
        
    Returns:
        List of (name, desc, value) tuples in file order
    """
    configs_in_order = []
    lines = content.split('\n')
    
    for line in lines:
        # Match #define CONFIG_NAME VALUE
        match = re.match(r'^\s*#define\s+(\w+)\s+(\d+)', line)
        if match:
            name = match.group(1)
            value = int(match.group(2))
            
            # Check if this is a known config
            if name in known_configs:
                desc, _ = known_configs[name]
                configs_in_order.append((name, desc, value))
            else:
                # Unknown config - add with generic description
                # Extract comment from same line if available
                comment_match = re.search(r'//\s*(.+)$', line)
                desc = comment_match.group(1).strip() if comment_match else 'Custom configuration'
                configs_in_order.append((name, desc, value))
    
    return configs_in_order


def parse_modules_config(filepath: str = 'Configuration/app_modules_config.h') -> ConfigFile:
    """
    Parse modules configuration file (app_modules_config.h).
    
    All values are integer (0 or 1 for toggles).
    Preserves the order of configs as they appear in the file.
    Configs are grouped by their actual section in the file, not by naming patterns.
    
    Args:
        filepath: Path to the modules config file
        
    Returns:
        ConfigFile object with parsed modules configuration
        
    Raises:
        FileOperationError: If file cannot be loaded
    """
    content = load_file(filepath)
    
    config_file = ConfigFile(filepath=filepath, original_content=content)
    
    # Define known configs with descriptions (for lookup)
    known_configs = {
        'ENABLE_HW_CAN': ('Enable/Disable CAN hardware driver', 'toggle'),
        'ENABLE_HW_UART': ('Enable/Disable UART hardware driver', 'toggle'),
        'ENABLE_HW_I2C': ('Enable/Disable I2C hardware driver', 'toggle'),
        'ENABLE_HW_GPIO': ('Enable/Disable GPIO hardware driver', 'toggle'),
        'ENABLE_SHELL': ('Enable/Disable Shell/Command Line Interface', 'toggle'),
        'ENABLE_LIBCSP': ('Enable/Disable libcsp library', 'toggle'),
        'ENABLE_LITTLEFS': ('Enable/Disable LittleFS file system', 'toggle'),
        'ENABLE_S25FL128L': ('Enable/Disable S25FL128L Flash driver', 'toggle'),
        'CSP_ENABLE_FILE': ('Enable/Disable CSP File Service', 'toggle'),
        'CSP_ENABLE_CONFIG': ('Enable/Disable CSP Configuration Service', 'toggle'),
        'CSP_ENABLE_THREAD': ('Enable/Disable CSP Thread Service', 'toggle'),
        'CSP_ENABLE_LOG': ('Enable/Disable CSP Logging Service', 'toggle'),
        'CSP_ENABLE_DATA_MON': ('Enable/Disable CSP Data Monitoring Service', 'toggle'),
        'CSP_ENABLE_DATA_MON_TEST': ('Enable/Disable CSP Data Monitor Test', 'toggle'),
        'CSP_ENABLE_ROUTE': ('Enable/Disable CSP Routing Service', 'toggle'),
        'CSP_ENABLE_REGISTER': ('Enable/Disable CSP Register Service', 'toggle'),
        'ENABLE_TIME_GNSS': ('Enable/Disable GNSS time synchronization', 'toggle'),
        'ENABLE_TIME_CTP': ('Enable/Disable CTP (CSP Time Protocol)', 'toggle'),
        'ENABLE_TIME_PPS': ('Enable/Disable PPS (Pulse Per Second) interrupt', 'toggle'),
        'ENABLE_SCRIPT_MANAGER': ('Enable/Disable Script Manager', 'toggle'),
        'ENABLE_BOOTLOADER_VALIDATOR': ('Enable/Disable Bootloader Validator', 'toggle'),
    }
    
    # Map section keywords to display names
    section_mapping = {
        'HARDWARE MODULES': '🔌 HARDWARE MODULES - โมดูลฮาร์ดแวร์',
        'SHELL': '💻 SHELL/CLI MODULES - โมดูล Shell',
        'CLI MODULES': '💻 SHELL/CLI MODULES - โมดูล Shell',
        'MIDDLEWARE': '⚙️ MIDDLEWARE MODULES - โมดูล Middleware',
        'CSP APPLICATION': '🌐 CSP APPLICATION MODULES - โมดูลแอปพลิเคชัน CSP',
        'TIME MANAGEMENT': '⏰ TIME MANAGEMENT MODULES - โมดูลจัดการเวลา',
        'AUTOMATION': '🤖 AUTOMATION MODULES - โมดูลออโตเมชัน',
        'BOOTLOADER VALIDATION': '🔧 BOOTLOADER VALIDATION - การตรวจสอบ Bootloader',
    }
    
    # Parse file line by line, tracking current section
    lines = content.split('\n')
    current_section = None
    sections_map = {}
    
    for line in lines:
        # Check if this is a section header
        # Match patterns like: /* ======================================================================== */
        # followed by: /*                        HARDWARE MODULES                                  */
        if re.match(r'^\s*/\*\s*=+\s*\*/', line):
            # This is a separator line, skip it
            continue
        
        # Check for section title line
        # Match: /*                        SECTION NAME                                  */
        section_match = re.match(r'^\s*/\*\s+(.+?)\s+\*/', line)
        if section_match:
            section_text = section_match.group(1).strip().upper()
            
            # Skip validation and other non-module sections
            if 'VALIDATION' in section_text or 'CONFIGURATION' in section_text:
                current_section = None
                continue
            
            # Map to display name
            current_section = None
            for keyword, display_name in section_mapping.items():
                if keyword in section_text:
                    current_section = display_name
                    if current_section not in sections_map:
                        sections_map[current_section] = []
                    break
            
            # If no match, create custom section
            if current_section is None and section_text and len(section_text) > 3:
                current_section = f"🔧 {section_text} - การตั้งค่าที่กำหนดเอง"
                if current_section not in sections_map:
                    sections_map[current_section] = []
            continue
        
        # Check if this is a config definition
        match = re.match(r'^\s*#define\s+(\w+)\s+(\d+)', line)
        if match:
            name = match.group(1)
            value = int(match.group(2))
            
            # Get description
            if name in known_configs:
                desc, _ = known_configs[name]
            else:
                # Extract comment from same line if available
                comment_match = re.search(r'//\s*(.+)$', line)
                desc = comment_match.group(1).strip() if comment_match else 'Custom configuration'
            
            # Add to current section (or default section if no section found)
            if current_section is None:
                current_section = '🔧 CUSTOM CONFIGURATIONS - การตั้งค่าที่กำหนดเอง'
                if current_section not in sections_map:
                    sections_map[current_section] = []
            
            sections_map[current_section].append((name, desc, value))
    
    # Create ConfigSection objects
    for section_name, configs in sections_map.items():
        if configs:  # Only add section if it has configs
            section_obj = ConfigSection(name=section_name)
            for name, desc, value in configs:
                config_obj = ToggleConfig(name=name, description=desc, value=value)
                section_obj.add_config(config_obj)
            config_file.add_section(section_obj)
    
    return config_file


def _parse_configs_in_order_with_types(content: str, known_configs: Dict[str, Tuple[str, str]]) -> List[Tuple[str, str, any, str]]:
    """
    Parse configs in the order they appear in the file (with types).
    
    Args:
        content: File content
        known_configs: Dict of {config_name: (description, type)}
        
    Returns:
        List of (name, desc, value, type) tuples in file order
    """
    configs_in_order = []
    lines = content.split('\n')
    
    for line in lines:
        # Match #define CONFIG_NAME VALUE (number)
        match = re.match(r'^\s*#define\s+(\w+)\s+(\d+)', line)
        if match:
            name = match.group(1)
            value = int(match.group(2))
            
            # Check if this is a known config
            if name in known_configs:
                desc, config_type = known_configs[name]
                configs_in_order.append((name, desc, value, config_type))
            else:
                # Unknown config - add with generic description
                comment_match = re.search(r'//\s*(.+)$', line)
                desc = comment_match.group(1).strip() if comment_match else 'Custom configuration'
                # Assume toggle for 0/1 values, otherwise number
                config_type = 'toggle' if value in [0, 1] else 'number'
                configs_in_order.append((name, desc, value, config_type))
            continue
        
        # Match #define CONFIG_NAME "STRING" (string)
        match = re.match(r'^\s*#define\s+(\w+)\s+"([^"]+)"', line)
        if match:
            name = match.group(1)
            value = match.group(2)
            
            # Check if this is a known config
            if name in known_configs:
                desc, config_type = known_configs[name]
                configs_in_order.append((name, desc, value, config_type))
            else:
                # Unknown string config
                comment_match = re.search(r'//\s*(.+)$', line)
                desc = comment_match.group(1).strip() if comment_match else 'Custom configuration'
                configs_in_order.append((name, desc, value, 'string'))
    
    return configs_in_order


def parse_params_config(filepath: str = 'Configuration/csp_app_params.h') -> ConfigFile:
    """
    Parse parameters configuration file (csp_app_params.h).
    
    Mixed types: integers for numbers, integers for toggles.
    Preserves the order of configs as they appear in the file.
    
    Args:
        filepath: Path to the params config file
        
    Returns:
        ConfigFile object with parsed params configuration
        
    Raises:
        FileOperationError: If file cannot be loaded
    """
    content = load_file(filepath)
    
    config_file = ConfigFile(filepath=filepath, original_content=content)
    
    # Define known configs with descriptions (for lookup)
    known_configs = {
        'CSP_APP_CUSTOM_PORT_1': ('Custom application port 1', 'number'),
        'CSP_APP_CUSTOM_PORT_2': ('Custom application port 2', 'number'),
        'CSP_APP_CUSTOM_PORT_3': ('Custom application port 3', 'number'),
        'CSP_APP_DEFAULT_TIMEOUT_MS': ('Default timeout (milliseconds)', 'number'),
        'CSP_APP_LONG_TIMEOUT_MS': ('Long timeout (milliseconds)', 'number'),
        'CSP_APP_RETRY_DELAY_MS': ('Retry delay (milliseconds)', 'number'),
        'CSP_APP_RX_BUFFER_SIZE': ('RX buffer size', 'number'),
        'CSP_APP_TX_BUFFER_SIZE': ('TX buffer size', 'number'),
        'CSP_APP_PACKET_POOL_SIZE': ('Packet pool size', 'number'),
        'CSP_APP_MAX_CONNECTIONS': ('Maximum connections', 'number'),
        'CSP_APP_CONNECTION_TIMEOUT_MS': ('Connection timeout (ms)', 'number'),
        'CSP_APP_MAX_RETRIES': ('Maximum retries', 'number'),
        'CSP_APP_RETRY_BACKOFF_MULTIPLIER': ('Retry backoff multiplier', 'number'),
        'CSP_APP_DEFAULT_NODE_ADDR': ('Default node address', 'number'),
        'CSP_APP_SUBNET_ADDR': ('Subnet address', 'number'),
        'CSP_APP_DEFAULT_ROUTE_ADDR': ('Default route address', 'number'),
        'CSP_APP_EVENT_QUEUE_SIZE': ('Event queue size', 'number'),
        'CSP_APP_CMD_QUEUE_SIZE': ('Command queue size', 'number'),
        'CSP_APP_ENABLE_LOOPBACK_TEST': ('Enable loopback test', 'toggle'),
        'CSP_APP_ENABLE_STATISTICS': ('Enable statistics', 'toggle'),
        'CSP_APP_ENABLE_DIAGNOSTICS': ('Enable diagnostics', 'toggle'),
        'CSP_APP_LOG_MAX_ENTRIES': ('Max log entries', 'number'),
        'CSP_APP_LOG_BUFFER_SIZE': ('Log buffer size', 'number'),
    }
    
    # Parse configs in file order
    configs_in_order = _parse_configs_in_order_with_types(content, known_configs)
    
    # Group configs by section based on naming patterns
    sections_map = {
        '🔌 APPLICATION-SPECIFIC PORTS - พอร์ตแอปพลิเคชัน': [],
        '⏱️ APPLICATION TIMEOUTS - เวลาหมดอายุ': [],
        '💾 APPLICATION BUFFER SIZES - ขนาด Buffer แอปพลิเคชัน': [],
        '🔗 APPLICATION CONNECTION SETTINGS - การตั้งค่า Connection': [],
        '🔄 APPLICATION RETRY SETTINGS - การตั้งค่า Retry': [],
        '📍 NODE ADDRESSING - การจัดการ Address': [],
        '📊 APPLICATION QUEUES - คิวแอปพลิเคชัน': [],
        '🚩 APPLICATION FEATURE FLAGS - ฟีเจอร์แอปพลิเคชัน': [],
        '📋 APPLICATION LOGGING - การบันทึก Log': [],
    }
    
    for name, desc, value, config_type in configs_in_order:
        if 'PORT' in name:
            sections_map['🔌 APPLICATION-SPECIFIC PORTS - พอร์ตแอปพลิเคชัน'].append((name, desc, value, config_type))
        elif 'TIMEOUT' in name or 'DELAY' in name:
            sections_map['⏱️ APPLICATION TIMEOUTS - เวลาหมดอายุ'].append((name, desc, value, config_type))
        elif 'BUFFER' in name or 'POOL' in name:
            sections_map['💾 APPLICATION BUFFER SIZES - ขนาด Buffer แอปพลิเคชัน'].append((name, desc, value, config_type))
        elif 'CONNECTION' in name:
            sections_map['🔗 APPLICATION CONNECTION SETTINGS - การตั้งค่า Connection'].append((name, desc, value, config_type))
        elif 'RETRIES' in name or 'RETRY' in name or 'BACKOFF' in name:
            sections_map['🔄 APPLICATION RETRY SETTINGS - การตั้งค่า Retry'].append((name, desc, value, config_type))
        elif 'ADDR' in name or 'NODE' in name or 'SUBNET' in name or 'ROUTE' in name:
            sections_map['📍 NODE ADDRESSING - การจัดการ Address'].append((name, desc, value, config_type))
        elif 'QUEUE' in name:
            sections_map['📊 APPLICATION QUEUES - คิวแอปพลิเคชัน'].append((name, desc, value, config_type))
        elif 'ENABLE' in name:
            sections_map['🚩 APPLICATION FEATURE FLAGS - ฟีเจอร์แอปพลิเคชัน'].append((name, desc, value, config_type))
        elif 'LOG' in name:
            sections_map['📋 APPLICATION LOGGING - การบันทึก Log'].append((name, desc, value, config_type))
    
    # Create ConfigSection objects (only non-empty sections)
    for section_name, configs in sections_map.items():
        if configs:  # Only add section if it has configs
            section_obj = ConfigSection(name=section_name)
            for name, desc, value, config_type in configs:
                if config_type == 'toggle':
                    config_obj = ToggleConfig(name=name, description=desc, value=value)
                else:
                    config_obj = NumberConfig(name=name, description=desc, value=value)
                section_obj.add_config(config_obj)
            config_file.add_section(section_obj)
    
    return config_file


def parse_library_config(filepath: str = 'Configuration/csp_library_config.h') -> ConfigFile:
    """
    Parse library configuration file (csp_library_config.h).
    
    Mixed types: integers, toggles, strings.
    Preserves the order of configs as they appear in the file.
    
    Args:
        filepath: Path to the library config file
        
    Returns:
        ConfigFile object with parsed library configuration
        
    Raises:
        FileOperationError: If file cannot be loaded
    """
    content = load_file(filepath)
    
    config_file = ConfigFile(filepath=filepath, original_content=content)
    
    # Define known configs with descriptions (for lookup)
    known_configs = {
        'CSP_DEBUG': ('Enable CSP debug', 'toggle'),
        'CSP_DEBUG_TIMESTAMP': ('Enable debug timestamp', 'toggle'),
        'CSP_USE_EXTERNAL_DEBUG': ('Use external debug', 'toggle'),
        'CSP_LOG_LEVEL_DEBUG': ('Log level debug', 'toggle'),
        'CSP_LOG_LEVEL_INFO': ('Log level info', 'toggle'),
        'CSP_LOG_LEVEL_WARN': ('Log level warn', 'toggle'),
        'CSP_LOG_LEVEL_ERROR': ('Log level error', 'toggle'),
        'CSP_USE_RDP': ('Use RDP', 'toggle'),
        'CSP_USE_RDP_FAST_CLOSE': ('RDP fast close', 'toggle'),
        'CSP_USE_CRC32': ('Use CRC32', 'toggle'),
        'CSP_USE_HMAC': ('Use HMAC', 'toggle'),
        'CSP_USE_XTEA': ('Use XTEA', 'toggle'),
        'CSP_USE_PROMISC': ('Use promiscuous mode', 'toggle'),
        'CSP_USE_QOS': ('Use QoS', 'toggle'),
        'CSP_USE_DEDUP': ('Use deduplication', 'toggle'),
        'CSP_DEFAULT_PORT': ('Default server port', 'number'),
        'CSP_DEFAULT_SERVER_NAME': ('Server name', 'string'),
        'CSP_DEFAULT_SERVER_PRIORITY': ('Server priority', 'string'),
        'CSP_DEFAULT_SERVER_STACK': ('Server stack size', 'number'),
        'CSP_DEFAULT_SERVER_AUTO_START': ('Auto start server', 'toggle'),
        'CSP_ROUTE_MANAGER_PORT': ('Route manager port', 'number'),
        'CSP_ROUTE_MANAGER_RPS_MTU': ('Route manager MTU', 'number'),
        'CSP_CTP_PORT': ('CTP port', 'number'),
        'CSP_CTP_CON_MAX': ('CTP max connections', 'number'),
        'CSP_CTP_SERVER_BROADCAST_PERIOD': ('Broadcast period', 'number'),
        'CSP_CTP_CLIENT_SERVER_ADDR': ('Client server address', 'number'),
        'CSP_CTP_CLIENT_UPDATE_PERIOD': ('Client update period', 'number'),
        'CSP_THREAD_MANAGER_PORT': ('Thread manager port', 'number'),
        'CSP_THREAD_MANAGER_CON_MAX': ('Max connections', 'number'),
        'CSP_THREAD_MANAGER_MAX_NUM': ('Max threads', 'number'),
        'CSP_THREAD_MANAGER_NAME_LENGTH': ('Thread name length', 'number'),
        'CSP_THREAD_MANAGER_RPS_MTU': ('Response MTU', 'number'),
        'CSP_CCP_PORT': ('Config manager port', 'number'),
        'CSP_CONFIG_MANAGER_CON_MAX': ('Max connections', 'number'),
        'CSP_CONFIG_MANAGER_RPS_MTU': ('Response MTU', 'number'),
        'CSP_CONFIG_SERVER_NAME': ('Config server name', 'string'),
        'CSP_CONFIG_SERVER_PRIORITY': ('Server priority', 'string'),
        'CSP_CONFIG_SERVER_STACK': ('Config server stack', 'number'),
        'CSP_CONFIG_SERVER_AUTO_START': ('Auto start', 'toggle'),
        'CSP_LOG_MANAGER_PORT': ('Log manager port', 'number'),
        'CSP_LOG_MANAGER_CON_MAX': ('Max connections', 'number'),
        'CSP_LOG_MANAGER_RPS_MTU': ('Response MTU', 'number'),
        'CSP_LOG_SERVER_NAME': ('Log server name', 'string'),
        'CSP_LOG_SERVER_PRIORITY': ('Server priority', 'string'),
        'CSP_LOG_SERVER_STACK': ('Log server stack', 'number'),
        'CSP_LOG_SERVER_AUTO_START': ('Auto start', 'toggle'),
        'CSP_FILE_MANAGER_PORT': ('File manager port', 'number'),
        'CSP_FILE_MANAGER_CON_MAX': ('Max connections', 'number'),
        'CSP_FILE_MANAGER_RPS_MTU': ('Response MTU', 'number'),
        'CSP_FILE_PATH_MAX': ('Max file path length', 'number'),
        'CSP_FILE_SERVER_NAME': ('File server name', 'string'),
        'CSP_FILE_SERVER_PRIORITY': ('Server priority', 'string'),
        'CSP_FILE_SERVER_STACK': ('File server stack', 'number'),
        'CSP_FILE_SERVER_AUTO_START': ('Auto start', 'toggle'),
        'CSP_FILE_MAX_RETRIES': ('Max file retries', 'number'),
        'CSP_FILE_TIMEOUT_MS': ('File timeout', 'number'),
        'CSP_FILE_ACK_TIMEOUT_MS': ('File ACK timeout', 'number'),
        'CSP_DATA_MONITOR_PORT': ('Data monitor port', 'number'),
        'CSP_DATA_MONITOR_CON_MAX': ('Max connections', 'number'),
        'CSP_DATA_MONITOR_MAX_TASK': ('Max tasks', 'number'),
        'CSP_DATA_MONITOR_NAME_LENGTH': ('Task name length', 'number'),
        'CSP_DATA_MONITOR_RPS_MTU': ('Response MTU', 'number'),
        'CSP_DATA_MONITOR_SERVER_NAME': ('Data monitor server name', 'string'),
        'CSP_DATA_MONITOR_SERVER_PRIORITY': ('Server priority', 'string'),
        'CSP_DATA_MONITOR_SERVER_STACK': ('Data monitor stack', 'number'),
        'CSP_DATA_MONITOR_SERVER_AUTO_START': ('Auto start', 'toggle'),
        'REGISTER_PORT_DEFAULT': ('Register service port', 'number'),
        'REG_SVC_SERVER_NAME': ('Register server name', 'string'),
        'REG_SVC_SERVER_PRIORITY': ('Server priority', 'string'),
        'REG_SVC_SERVER_STACK': ('Register server stack', 'number'),
        'REG_SVC_SERVER_AUTO_START': ('Auto start', 'toggle'),
        'REG_SVC_CON_BACKLOG': ('Connection backlog', 'number'),
        'REG_SVC_ACCEPT_TIMEOUT_MS': ('Accept timeout', 'number'),
        'REG_SVC_READ_TIMEOUT_MS': ('Read timeout', 'number'),
        'CONFIG_CSP_UART_DEVICE_NUM': ('UART device number', 'number'),
        'CONFIG_CSP_UART_RX_BUFFER_LENGTH': ('UART RX buffer length', 'number'),
        'CONFIG_CSP_UART_TX_BUFFER_LENGTH': ('UART TX buffer length', 'number'),
        'CONFIG_CSP_CAN_DEVICE_NUM': ('CAN device number', 'number'),
    }
    
    sections_data = [
        {
            'name': '🔒 CSP PROTOCOL FEATURES - ฟีเจอร์โปรโตคอล CSP',
            'configs': [
                ('CSP_USE_RDP', 'Use RDP (Reliable Data Protocol)', 1, 'toggle'),
                ('CSP_USE_RDP_FAST_CLOSE', 'RDP fast close', 0, 'toggle'),
                ('CSP_USE_CRC32', 'Use CRC32 checksum', 1, 'toggle'),
                ('CSP_USE_HMAC', 'Use HMAC', 1, 'toggle'),
                ('CSP_USE_XTEA', 'Use XTEA encryption', 1, 'toggle'),
                ('CSP_USE_PROMISC', 'Use promiscuous mode', 1, 'toggle'),
                ('CSP_USE_QOS', 'Use QoS (Quality of Service)', 0, 'toggle'),
                ('CSP_USE_DEDUP', 'Use deduplication', 1, 'toggle')
            ]
        },
        {
            'name': '🌐 DEFAULT SERVER CONFIGURATION - การตั้งค่า Default Server',
            'configs': [
                ('CSP_DEFAULT_PORT', 'Default server port', 10, 'number'),
                ('CSP_DEFAULT_SERVER_NAME', 'Server name', '"CSPDefServ"', 'string'),
                ('CSP_DEFAULT_SERVER_PRIORITY', 'Server priority', 'osPriorityNormal', 'string'),
                ('CSP_DEFAULT_SERVER_STACK', 'Server stack size', 2048, 'number'),
                ('CSP_DEFAULT_SERVER_AUTO_START', 'Auto start server', 1, 'toggle')
            ]
        },
        {
            'name': '📡 ROUTE MANAGER CONFIGURATION - การตั้งค่า Route Manager',
            'configs': [
                ('CSP_ROUTE_MANAGER_PORT', 'Route manager port', 9, 'number'),
                ('CSP_ROUTE_MANAGER_RPS_MTU', 'Route manager MTU', 254, 'number')
            ]
        },
        {
            'name': '🕐 TIME PROTOCOL (CTP) CONFIGURATION - การตั้งค่า Time Protocol',
            'configs': [
                ('CSP_CTP_PORT', 'CTP port', 22, 'number'),
                ('CSP_CTP_CON_MAX', 'CTP max connections', 5, 'number'),
                ('CSP_CTP_SERVER_BROADCAST_PERIOD', 'Broadcast period (0=disable)', 0, 'number'),
                ('CSP_CTP_CLIENT_SERVER_ADDR', 'Client server address', 9, 'number'),
                ('CSP_CTP_CLIENT_UPDATE_PERIOD', 'Client update period (ms)', 60000, 'number')
            ]
        },
        {
            'name': '🧵 THREAD MANAGER CONFIGURATION - การตั้งค่า Thread Manager',
            'configs': [
                ('CSP_THREAD_MANAGER_PORT', 'Thread manager port', 20, 'number'),
                ('CSP_THREAD_MANAGER_CON_MAX', 'Max connections', 5, 'number'),
                ('CSP_THREAD_MANAGER_MAX_NUM', 'Max threads', 10, 'number'),
                ('CSP_THREAD_MANAGER_NAME_LENGTH', 'Thread name length', 16, 'number'),
                ('CSP_THREAD_MANAGER_RPS_MTU', 'Response MTU', 196, 'number')
            ]
        },
        {
            'name': '⚙️ CONFIG MANAGER CONFIGURATION - การตั้งค่า Config Manager',
            'configs': [
                ('CSP_CCP_PORT', 'Config manager port', 19, 'number'),
                ('CSP_CONFIG_MANAGER_CON_MAX', 'Max connections', 5, 'number'),
                ('CSP_CONFIG_MANAGER_RPS_MTU', 'Response MTU', 224, 'number'),
                ('CSP_CONFIG_SERVER_NAME', 'Config server name', '"ConfigDefServ"', 'string'),
                ('CSP_CONFIG_SERVER_PRIORITY', 'Server priority', 'osPriorityNormal', 'string'),
                ('CSP_CONFIG_SERVER_STACK', 'Config server stack', 4096, 'number'),
                ('CSP_CONFIG_SERVER_AUTO_START', 'Auto start', 1, 'toggle')
            ]
        },
        {
            'name': '📋 LOG MANAGER CONFIGURATION - การตั้งค่า Log Manager',
            'configs': [
                ('CSP_LOG_MANAGER_PORT', 'Log manager port', 18, 'number'),
                ('CSP_LOG_MANAGER_CON_MAX', 'Max connections', 5, 'number'),
                ('CSP_LOG_MANAGER_RPS_MTU', 'Response MTU', 224, 'number'),
                ('CSP_LOG_SERVER_NAME', 'Log server name', '"LogDefServ"', 'string'),
                ('CSP_LOG_SERVER_PRIORITY', 'Server priority', 'osPriorityNormal', 'string'),
                ('CSP_LOG_SERVER_STACK', 'Log server stack', 1024, 'number'),
                ('CSP_LOG_SERVER_AUTO_START', 'Auto start', 1, 'toggle')
            ]
        },
        {
            'name': '📁 FILE MANAGER CONFIGURATION - การตั้งค่า File Manager',
            'configs': [
                ('CSP_FILE_MANAGER_PORT', 'File manager port', 17, 'number'),
                ('CSP_FILE_MANAGER_CON_MAX', 'Max connections', 10, 'number'),
                ('CSP_FILE_MANAGER_RPS_MTU', 'Response MTU', 238, 'number'),
                ('CSP_FILE_PATH_MAX', 'Max file path length', 192, 'number'),
                ('CSP_FILE_SERVER_NAME', 'File server name', '"FileDefServ"', 'string'),
                ('CSP_FILE_SERVER_PRIORITY', 'Server priority', 'osPriorityNormal', 'string'),
                ('CSP_FILE_SERVER_STACK', 'File server stack', 5120, 'number'),
                ('CSP_FILE_SERVER_AUTO_START', 'Auto start', 1, 'toggle'),
                ('CSP_FILE_MAX_RETRIES', 'Max file retries', 5, 'number'),
                ('CSP_FILE_TIMEOUT_MS', 'File timeout (ms)', 2000, 'number'),
                ('CSP_FILE_ACK_TIMEOUT_MS', 'File ACK timeout (ms)', 1000, 'number')
            ]
        },
        {
            'name': '📊 DATA MONITOR CONFIGURATION - การตั้งค่า Data Monitor',
            'configs': [
                ('CSP_DATA_MONITOR_PORT', 'Data monitor port', 16, 'number'),
                ('CSP_DATA_MONITOR_CON_MAX', 'Max connections', 5, 'number'),
                ('CSP_DATA_MONITOR_MAX_TASK', 'Max tasks', 5, 'number'),
                ('CSP_DATA_MONITOR_NAME_LENGTH', 'Task name length', 16, 'number'),
                ('CSP_DATA_MONITOR_RPS_MTU', 'Response MTU', 224, 'number'),
                ('CSP_DATA_MONITOR_SERVER_NAME', 'Data monitor server name', '"DataMonDefServ"', 'string'),
                ('CSP_DATA_MONITOR_SERVER_PRIORITY', 'Server priority', 'osPriorityNormal', 'string'),
                ('CSP_DATA_MONITOR_SERVER_STACK', 'Data monitor stack', 2048, 'number'),
                ('CSP_DATA_MONITOR_SERVER_AUTO_START', 'Auto start', 1, 'toggle')
            ]
        },
        {
            'name': '📝 REGISTER SERVICE CONFIGURATION - การตั้งค่า Register Service',
            'configs': [
                ('REGISTER_PORT_DEFAULT', 'Register service port', 24, 'number'),
                ('REG_SVC_SERVER_NAME', 'Register server name', '"RegSvcDef"', 'string'),
                ('REG_SVC_SERVER_PRIORITY', 'Server priority', 'osPriorityHigh', 'string'),
                ('REG_SVC_SERVER_STACK', 'Register server stack', 1024, 'number'),
                ('REG_SVC_SERVER_AUTO_START', 'Auto start', 1, 'toggle'),
                ('REG_SVC_CON_BACKLOG', 'Connection backlog', 10, 'number'),
                ('REG_SVC_ACCEPT_TIMEOUT_MS', 'Accept timeout (ms)', 1000, 'number'),
                ('REG_SVC_READ_TIMEOUT_MS', 'Read timeout (ms)', 10, 'number')
            ]
        },
        {
            'name': '📡 KISS INTERFACE CONFIGURATION - การตั้งค่า KISS Interface',
            'configs': [
                ('CONFIG_CSP_UART_DEVICE_NUM', 'UART device number', 1, 'number'),
                ('CONFIG_CSP_UART_RX_BUFFER_LENGTH', 'UART RX buffer length', 512, 'number'),
                ('CONFIG_CSP_UART_TX_BUFFER_LENGTH', 'UART TX buffer length', 256, 'number'),
                ('CONFIG_CSP_CAN_DEVICE_NUM', 'CAN device number', 1, 'number')
            ]
        }
    ]
    
    # Parse values from content
    for section in sections_data:
        for i, (name, desc, default, config_type) in enumerate(section['configs']):
            if config_type == 'string':
                # Exact regex pattern from original for strings: #define\s+{name}\s+"([^"]+)"
                match = re.search(rf'#define\s+{name}\s+"([^"]+)"', content)
            else:
                # Exact regex pattern from original for numbers: #define\s+{name}\s+(\d+)
                match = re.search(rf'#define\s+{name}\s+(\d+)', content)
            
            if match:
                value = match.group(1) if config_type == 'string' else int(match.group(1))
                section['configs'][i] = (name, desc, value, config_type)
    
    # Create ConfigSection objects
    for section in sections_data:
        section_obj = ConfigSection(name=section['name'])
        for name, desc, value, config_type in section['configs']:
            if config_type == 'toggle':
                config_obj = ToggleConfig(name=name, description=desc, value=value)
            elif config_type == 'string':
                config_obj = StringConfig(name=name, description=desc, value=value)
            else:
                config_obj = NumberConfig(name=name, description=desc, value=value)
            section_obj.add_config(config_obj)
        config_file.add_section(section_obj)
    
    return config_file


def parse_ksdef_config(filepath: str = 'Configuration/ksdef.h') -> ConfigFile:
    """
    Parse ksdef.h configuration file.
    
    Dynamically discovers all defines in the Private defines section.
    
    Args:
        filepath: Path to the ksdef config file
        
    Returns:
        ConfigFile object with parsed ksdef configuration
        
    Raises:
        FileOperationError: If file cannot be loaded
    """
    content = load_file(filepath)
    
    config_file = ConfigFile(filepath=filepath, original_content=content)
    
    # Known configs with descriptions
    known_configs = {
        'TSC_USING_HEAP': ('Use heap memory management', 'toggle'),
        'TSC_USING_FINSH': ('Use FINSH shell', 'toggle'),
        'TSC_USING_CONSOLE': ('Use console', 'toggle'),
        'TSC_KSERVICE_USING_STDLIB': ('Use standard library', 'toggle'),
        'TSC_DEBUG': ('Enable debug mode', 'toggle'),
        'TSC_ALIGN_SIZE': ('Alignment size', 'number'),
        'TSC_CONSOLEBUF_SIZE': ('Console buffer size', 'number')
    }

    # Extract the Private defines section
    section_match = re.search(r'/\* USER CODE BEGIN Private defines \*/(.*?)/\* USER CODE END Private defines \*/', content, re.DOTALL)
    if not section_match:
        return config_file
        
    section_content = section_match.group(1)
    
    kernel_section = ConfigSection(name='⚙️ TSC KERNEL SETTINGS - การตั้งค่า Kernel')
    
    lines = section_content.split('\n')
    for line in lines:
        # Match #define NAME VALUE (number)
        match = re.match(r'^\s*#define\s+(\w+)\s+(\d+)', line)
        if match:
            name, value = match.group(1), int(match.group(2))
            desc, config_type = known_configs.get(name, ('Kernel configuration', 'number' if value not in [0, 1] else 'toggle'))
            if config_type == 'toggle':
                kernel_section.add_config(ToggleConfig(name, desc, value))
            else:
                kernel_section.add_config(NumberConfig(name, desc, value))
            continue
            
        # Match #define NAME "VALUE" (string)
        match = re.match(r'^\s*#define\s+(\w+)\s+"([^"]+)"', line)
        if match:
            name, value = match.group(1), match.group(2)
            desc, _ = known_configs.get(name, ('Kernel configuration', 'string'))
            kernel_section.add_config(StringConfig(name, desc, value))
            continue
            
        # Match #define NAME (toggle - enabled)
        match = re.match(r'^\s*#define\s+(\w+)\s*$', line)
        if match:
            name = match.group(1)
            desc, _ = known_configs.get(name, ('Kernel configuration', 'toggle'))
            kernel_section.add_config(ToggleConfig(name, desc, 1))
            continue
            
        # Match // #define NAME (toggle - disabled)
        match = re.match(r'^\s*//\s*#define\s+(\w+)', line)
        if match:
            name = match.group(1)
            desc, _ = known_configs.get(name, ('Kernel configuration', 'toggle'))
            kernel_section.add_config(ToggleConfig(name, desc, 0))
            continue

    if kernel_section.configs:
        config_file.add_section(kernel_section)
    
    return config_file
