# STM32L496 Hardware-In-Loop (HIL) Testing Guide

This guide provides the instructions for onboarding new HIL runners, configuring CI pipelines, and reproducing smoke tests locally for the ThaiSpace-AstraCore-STM32 project.

## 1. Prerequisites (Runner & Local Setup)

To run HIL tests, the host machine (Windows) must have the following tools installed and accessible in the system PATH:

- **STM32CubeCLT (Command Line Tools):** Includes compilers and programming utilities.
- **STM32_Programmer_CLI:** Specifically required for flashing the firmware to the target.
- **PowerShell 5.1+:** For running the smoke test orchestration scripts.
- **Git Bash / Bash environment:** For building using the provided Makefiles.
- **Serial Drivers:** Ensure the ST-Link Virtual COM Port driver is installed.

## 2. Hardware Wiring & Setup

The HIL runner requires a physical **STM32L496ZGT6 (Nucleo-144)** board connected via USB:

1. **CN1 (USB ST-LINK):** Connect to the host machine. This provides both the debugging/flashing interface and the Virtual COM Port (VCP).
2. **Jumpers:** Ensure default Nucleo jumpers are set (specifically JP1, JP5, and JP6 for ST-LINK power and communication).
3. **COM Port:** Identify the port number (e.g., `COM6`) in the Windows Device Manager.

## 3. GitLab CI Configuration

The following **CI/CD Variables** must be defined in the GitLab project settings for the runner to operate:

| Variable | Description | Example Value |
|----------|-------------|---------------|
| `HIL_COM_PORT` | The serial port assigned to the Nucleo board. | `COM6` |
| `HIL_COM_BAUD` | Serial baud rate for the TSC Shell. | `115200` |
| `STM32_PROG_CLI` | Path to `STM32_Programmer_CLI.exe`. | `C:\ST\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe` |

*Note: Ensure the GitLab Runner has the `hil` tag to target the machine with physical hardware connected.*

## 4. Developer Workflow (Local Reproduction)

### Step A: Build the Project
Open Git Bash in the `STM32L496` directory and run:
```bash
make clean
make -j$(nproc)
```

### Step B: Flash the Firmware
Use the following command (adjust path to `.elf` and programmer):
```powershell
& "C:\Path\To\STM32_Programmer_CLI.exe" -c port=SWD -w .\build\STM32L496.elf -v -rst
```

### Step C: Run Smoke Tests
Execute the PowerShell script to run all application manifests:
```powershell
.\scripts\run_smoke_tests.ps1 `
    -com_port "COM6" `
    -com_baud 115200 `
    -project_dir "." `
    -report_path ".\reports\hil_junit.xml" `
    -log_path ".\logs\smoke_test.log"
```

## 5. Troubleshooting & Common Failures

### 1. "Serial port access denied"
- **Cause:** Another program (e.g., Putty, TeraTerm) is holding the COM port open.
- **Solution:** Close all other serial terminals before starting the test.

### 2. "Flash failure / Target not found"
- **Cause:** ST-Link is in a hung state or busy.
- **Solution:** Unplug and replug the USB cable, or press the Black (Reset) button on the Nucleo.

### 3. "Pattern not found" (Smoke Test Failures)
- **Cause:** Prompt mismatch or character buffer leakage.
- **Solution:** The runner script now includes `Wait-For-Prompt` and `Flush-Buffer` logic. If it fails, check `logs\smoke_test.log` to see if the board produced a "command not found" error due to truncated input.

### 4. Shell Startup Delay
- **Issue:** The TSC Shell (RT-Thread) might take a few hundred milliseconds to initialize after reset.
- **Solution:** The script waits 1 second and sends an initial Newline to clear any boot-up noise.

---
**Maintained by:** ThaiSpace-AstraCore Team
**Last Updated:** March 2026
