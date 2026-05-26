# ADCS Controller

Firmware for an STM32H723ZGTx / NUCLEO-H723ZG board running an ADCS controller on top of STM32 HAL, CMSIS-RTOS2, and FreeRTOS.

The application-specific ADCS code is intentionally kept in `Core/Inc/adcs` as header-only modules so CubeMX/CubeIDE generated source files can remain mostly untouched.

## Source Layout

- `Core/Src/main.cpp` - MCU startup, cache, HAL, clock, GPIO, USART3, and RTOS kernel startup.
- `Core/Src/freertos.cpp` - ADCS application bootstrap and FreeRTOS failure callbacks.
- `Core/Inc/adcs/adcs_config.h` - ADCS timing, stack sizes, priorities, gains, limits, and telemetry settings.
- `Core/Inc/adcs/adcs_types.h` - Shared ADCS data types using fixed-size Eigen vectors.
- `Core/Inc/adcs/adcs_platform.h` - HAL-facing board helpers for tick, safe request, and UART TX.
- `Core/Inc/adcs/adcs_app.h` - FreeRTOS task graph, queues, shared state, mode logic, and health monitoring.
- `Core/Inc/adcs/adcs_controller.h` - High-level controller dispatch for detumble and sun-point behavior.
- `Core/Inc/adcs/adcs_magnetic_detumble.h` - Magnetic detumble dipole command helper.
- `Core/Inc/adcs/adcs_bdot.h` and `Core/Inc/adcs/adcs_rate_dumping.h` - Optional controller helpers not wired into the current default task graph.
- `Drivers/` and `Middlewares/` - STM32 HAL, BSP, CMSIS, and FreeRTOS vendor code.
- `Debug/` - CubeIDE generated makefiles. Object files and binaries are build artifacts and are ignored.
- `docs/` - Project notes and generated-document helpers.

## Runtime Flow

`main.cpp` initializes the MCU and starts the RTOS scheduler. `freertos.cpp` calls `adcs::app::Initialize()`, which creates queues, mutexes, event flags, UART telemetry, initial state, and five RTOS tasks.

Current task graph:

- `ADCS_SENSOR` - publishes a fail-safe placeholder `SensorSample` every 100 ms until a real sensor source is wired in.
- `ADCS_NAV` - filters body rate, selects ADCS mode, and publishes `NavigationSolution`.
- `ADCS_CTRL` - consumes the latest navigation solution every 100 ms, computes actuator command, and stores it in shared state.
- `ADCS_TLM` - emits text telemetry snapshots when events occur or every 1000 ms.
- `ADCS_HEALTH` - checks sensor availability, task heartbeats, queue overruns, and safe/fault state every 250 ms.

## Hardware Interface

- USART3 baud rate: `460800`
- USART3 TX/RX pins: `PD8` / `PD9`
- User safe request input: `PC13` (`B1`)
- LEDs: `PB0` green, `PB14` red, `PE1` yellow
- HAL time base: `TIM7`

USART3 is currently used only for bounded text telemetry. The previous binary simulator link was removed.

## Mode Behavior

Mode selection is implemented in `adcs_app.h`.

- `Fault` if the sensor sample is unavailable or a monitored task/queue fails.
- `Safe` if the user safe input is active.
- `Startup` only if `kStartupModeDurationMs` is set above zero. It is currently `0`, so startup is only the initial state.
- `Detumble` if filtered body-rate norm is at or above `0.05 rad/s`, with hysteresis down to `0.00349066 rad/s`.
- Otherwise the current code returns `Safe`.

`SunPoint` exists in the enum and controller branch, but the current mode selector does not enter it by default.

## Build And Flash

Open this folder in VSCode or STM32CubeIDE.

VSCode tasks are defined in `.vscode/tasks.json`:

- `STM32: Build Debug`
- `STM32: Flash Debug`
- `STM32: Build + Flash Debug`
- `STM32: ST-LINK Info`

The tasks are pinned to STM32CubeIDE 1.16.1 tool paths on this machine. If CubeIDE moves or is upgraded, update `.vscode/tasks.json`.

The build currently expects Eigen at `C:/Users/Chinnapop/Downloads/eigen-3.4.0`.
