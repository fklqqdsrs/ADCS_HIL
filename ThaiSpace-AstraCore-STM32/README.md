# ThaiSpace AstraCore STM32 Framework

A flight-grade embedded software framework for STM32-based satellite and aerospace applications. This repository contains the core libraries, application modules, and hardware abstraction layers for the AstraCore system.

## 🌟 Key Features
- **Modular Architecture:** Decoupled application modules and core kernel services.
- **Flight-Grade Reliability:** Robust file system (LittleFS), CSP protocol, and strict error handling.
- **CI/CD Driven:** Automated build and Hardware-in-the-Loop (HIL) testing on every push.
- **Hardened Runner Support:** Portable CI configuration with explicit error checking and firmware safety gates.
- **Tooling Support:** Seamless integration with `system-configurator` for project assembly and parameter management.

## 🏗️ Project Structure
- `Applications/`: Reusable application-level modules (Log, File, Data Monitor, etc.)
- `Core/`: Kernel services, Shell (MSH), and system initialization code.
- `Drivers/`: Hardware-specific drivers (e.g., S25FL128L Flash).
- `Middlewares/`: Third-party and custom middleware (libcsp, littlefs).
- `STM32L496/`: Canonical project scaffold and build target.

## 🛠️ Getting Started

### Prerequisites
- [ARM GNU Toolchain](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain)
- [Make](https://www.gnu.org/software/make/)
- [Python 3.8+](https://www.python.org/)
- [system-configurator](https://git.narit.or.th/tsc-1/cicd-devops/system-configurator)
- [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html) (for HIL)

### Build Procedure
1. Clone this repository.
2. Clone `system-configurator` alongside this repo.
3. Assemble the project:
   ```bash
   python system-configurator/unified_config_manager_cli.py export STM32L496 --fw-src .
   ```
4. Build the target:
   ```bash
   cd STM32L496
   make -j16 all
   ```

## 🚀 CI/CD and Smoke Testing
This project uses GitLab CI to perform automated testing:
- **HIL Testing:** Automated testing on real hardware. Runner-specific settings (COM port, ST-LINK SN) are managed via CI Variables. See [HIL Testing Guide](HIL_TESTING_GUIDE.md) for more info.
- **Smoke Tests:** Automated CLI command verification using `smoke_manifest.json` and `run_smoke_tests.ps1`. These tests ensure that all registered shell commands are functioning and returning expected outputs.
- **Fail-Fast Policy:** Pipeline stages are configured to fail immediately on export or build errors to ensure no stale artifacts are used.

## 📄 License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---
© 2026 National Astronomical Research Institute of Thailand (NARIT)
--- End of content ---