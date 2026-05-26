#include "s25fl128l_littlefs.h"
#include <stdio.h>
#include <stdbool.h>
#include <kservice.h>

#if ENABLE_S25FL128L && ENABLE_LITTLEFS

/**
 * @brief Run a sequence of tests on the S25FL128L flash to verify driver integrity.
 * 
 * Note: On evaluation boards without physical NOR Flash, this test is expected to 
 * fail but should correctly report S25FL128L_Timeout or S25FL128L_Err instead of hanging.
 */
static void flash_smoke_test_cmd(int argc, char** argv) {
    tsc_kprintf("\n--- S25FL128L Driver Smoke Test ---\r\n");

    // 1. Read JEDEC ID
    // This verifies the SPI contract fix and ID reading logic.
    tsc_kprintf("[1/4] Reading JEDEC ID...\r\n");
    S25FL128L_result_t res = s25fl128l_read_id(&s25fl128l_handle);
    
    if (res == S25FL128L_Ok) {
        tsc_kprintf("  SUCCESS: MID=0x%02X, DID=0x%04X\r\n", 
                    s25fl128l_handle.manufacturer_id, 
                    s25fl128l_handle.device_id);
    } else {
        tsc_kprintf("  EXPECTED FAILURE (if no chip): Result=%d (1=Err, 2=Timeout)\r\n", res);
        // Even if ID read fails, we can check if it timed out properly (propagated from HAL)
    }

    // 2. Sector Erase Test
    uint32_t test_addr = 0x00F00000; // 15MB mark (safe area)
    tsc_kprintf("[2/4] Testing Sector Erase at 0x%08X...\r\n", test_addr);
    res = s25fl128l_erase_sector(&s25fl128l_handle, test_addr, 2000);
    tsc_kprintf("  Result: %d\r\n", res);

    // 3. Write/Read Test
    uint8_t tx_data[16] = {0xAA, 0x55, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
    uint8_t rx_data[16] = {0};
    
    tsc_kprintf("[3/4] Testing Page Program (16 bytes)...\r\n");
    res = s25fl128l_write(&s25fl128l_handle, test_addr, tx_data, 16, 1000);
    tsc_kprintf("  Write Result: %d\r\n", res);

    tsc_kprintf("[4/4] Testing Data Verification...\r\n");
    res = s25fl128l_read(&s25fl128l_handle, test_addr, rx_data, 16, 1000);
    
    if (res == S25FL128L_Ok) {
        bool match = true;
        for (int i = 0; i < 16; i++) {
            if (rx_data[i] != tx_data[i]) {
                match = false;
                break;
            }
        }
        if (match) {
            tsc_kprintf("  PASSED: Data Integrity Verified.\r\n");
        } else {
            tsc_kprintf("  FAILED: Data Mismatch detected.\r\n");
        }
    } else {
        tsc_kprintf("  FAILED: Could not read back data. Result: %d\r\n", res);
    }

    tsc_kprintf("--- Smoke Test Completed ---\n\r");
}

#include <finsh.h>
/**
 * Register the command to the system shell.
 * Usage: flash_smoke_test
 */
TSC_CMD_EXPORT_ALIAS(flash_smoke_test_cmd, flash_smoke_test, Run S25FL128L Flash Smoke Test);

#endif /* ENABLE_S25FL128L */
