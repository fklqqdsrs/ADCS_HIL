# Changelog

All notable changes to this project will be documented in this file.

## [2.2.0] - 2026-03-18

### Fixed
- **S25FL128L Driver:** Fixed SPI transaction contract and JEDEC ID read logic to ensure reliable flash identification and communication.
- **Logging System:** Repaired `TSC_LOG` functionality following regressions introduced by SD card integration changes.

## [2.1.0-hil-hardening] - 2026-03-13

### Added
- Comprehensive `HIL_TESTING_GUIDE.md` for runner onboarding and local reproduction.
- Firmware size enforcement in CI to prevent flashing oversized binaries.
- Advisory Lint stage with persistent artifacts for quality monitoring.
- JUnit reporting integration for granular smoke test results in GitLab.

### Fixed
- **Runner Stability:** Fixed serial buffer leakage and race conditions in `run_smoke_tests.ps1` by implementing reliable prompt synchronization.
- **CI Reliability:** Implemented "Fail-Fast" logic in PowerShell scripts to catch export and build errors immediately.
- **Portability:** Removed hardcoded COM ports and tool paths from repo files; moved to GitLab CI Variables and prioritized resolution logic.

### Changed
- Refactored `hil.config.json` to act as project-wide hardware defaults rather than runner-specific config.
- Updated all application smoke manifests to match actual TSC-1 Shell command outputs.

## [2.0.0-astracore] - 2026-03-13

### Added
- Dynamic parsing for `ksdef.h` in `system-configurator` to support all kernel defines.
- Extended `smoke_manifest.json` for all application modules.

### Fixed
- Incorrect `expect` patterns in `smoke_manifest.json` for `time`, `service_automate`, and `config` modules.
- Updated `config_whoami` expectation to match lowercase "guest" user.
- Updated `sched_list` expectation to match actual table header output.
- Updated `csp_time_status` expectation to match actual CTP status output.

## [1.0.0-baseline] - 2026-03-11

### Added
- Canonical STM32L496 project scaffold (Makefile, Linker, Startup).
- Automated CI/CD pipeline with Decoupled Build Workflow.
- Hardware-in-the-Loop (HIL) automated testing suite.
- Integration with `system-configurator` for project assembly.
- Comprehensive hardware documentation and pin mapping.

### Fixed
- Linker errors related to standard `assert` in `msh.c`.
- Missing error codes in `file_core.h` for CSP file protocol.
- Version mismatch in application-level library headers.

### Changed
- Decoupled `system-configurator` from being a submodule to a standalone tool.
- Improved build assembly logic to merge configurations dynamically.
