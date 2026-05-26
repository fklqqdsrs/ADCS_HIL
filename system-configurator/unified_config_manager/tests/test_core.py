import unittest
import os
from unified_config_manager.core.models import (
    ToggleConfig, NumberConfig, StringConfig, ConfigSection, ConfigFile, ConfigType
)

class TestConfigModels(unittest.TestCase):
    def test_toggle_config(self):
        item = ToggleConfig("TEST_TOGGLE", "Description", 1)
        self.assertEqual(item.name, "TEST_TOGGLE")
        self.assertTrue(item.value)
        self.assertEqual(item.int_value, 1)
        self.assertEqual(item.config_type, ConfigType.TOGGLE)
        
        item.value = False
        self.assertEqual(item.int_value, 0)

    def test_number_config(self):
        item = NumberConfig("TEST_NUMBER", "Description", 123)
        self.assertEqual(item.value, 123)
        self.assertEqual(item.config_type, ConfigType.NUMBER)

    def test_string_config(self):
        item = StringConfig("TEST_STRING", "Description", '"quoted"')
        self.assertEqual(item.value, "quoted")
        self.assertEqual(item.quoted_value, '"quoted"')
        self.assertEqual(item.config_type, ConfigType.STRING)

class TestConfigStructure(unittest.TestCase):
    def test_section_and_file(self):
        section = ConfigSection("Section 1")
        item = ToggleConfig("ITEM1", "Desc", 1)
        section.add_config(item)
        
        self.assertEqual(len(section.configs), 1)
        self.assertEqual(section.get_config("ITEM1"), item)
        
        config_file = ConfigFile("test.h")
        config_file.add_section(section)
        
        self.assertEqual(len(config_file.sections), 1)
        self.assertEqual(config_file.get_config("ITEM1"), item)

if __name__ == '__main__':
    unittest.main()
