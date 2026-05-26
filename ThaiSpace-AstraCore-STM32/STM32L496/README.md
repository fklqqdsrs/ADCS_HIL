# STM32L496 Base Project Baseline

This directory contains the canonical project scaffold for the ThaiSpace AstraCore framework, targeting the STM32L496 microcontroller.

## 📋 Hardware Specifications
- **Target Board:** NUCLEO-L496ZG
- **MCU:** STM32L496ZGT6 (Cortex-M4, 1MB Flash, 320KB RAM)
- **UART Console:** LPUART1 (connected to ST-LINK Virtual COM Port)
- **Baud Rate:** 115200

## 🛠️ Build & Flash Procedure

### Manual Build
Ensure you have the ARM GNU Toolchain and Make installed.
```bash
# 1. Assemble project files (requires system-configurator)
python ../system-configurator/unified_config_manager_cli.py export . --fw-src ..

# 2. Compile
make -j16 all
```

### Manual Flash (using STM32CubeProgrammer CLI)
```bash
"C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe" -c port=SWD mode=UR reset=HWrst -w build/STM32L496ZGTx_TSC1.bin 0x08000000 -v -rst
```

## 🚀 CI/CD Integration
This project is fully integrated with GitLab CI. Every push triggers:
1. **Static Analysis:** via `cppcheck`.
2. **Automated Assembly:** Merging core framework and configurations.
3. **Firmware Compilation:** via `make`.
4. **HIL Test:** Automated flashing and UART verification on real hardware.

## 📌 Pin Mapping (Minimal BSP)
- **LPUART1_TX:** PG7 (ST-LINK VCP)
- **LPUART1_RX:** PG8 (ST-LINK VCP)
- **LD1 (Green LED):** PB0
- **LD2 (Blue LED):** PB7
- **LD3 (Red LED):** PB14
