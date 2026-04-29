# คู่มือโครงสร้างและโหมดของ ADCS Controller

เอกสารนี้อธิบายตาม source code ปัจจุบันใน `Core/Inc/adcs` ไม่ยึดตาม behavior เก่าที่เคยอยู่ในเอกสารเดิม

## ภาพรวมระบบ

โปรเจคนี้เป็น firmware สำหรับ STM32H723ZGTx / NUCLEO-H723ZG ทำงานบน STM32 HAL, CMSIS-RTOS2 และ FreeRTOS โดย logic ของ ADCS อยู่ใน header ใต้ `Core/Inc/adcs`

ลำดับเริ่มระบบคือ:

1. `Core/Src/main.cpp` เปิด cache, init HAL, clock, GPIO, USART3 และ RTOS kernel
2. `Core/Src/freertos.cpp` เรียก `adcs::app::Initialize()`
3. `adcs_app.h` สร้าง RTOS objects, initial state และ task graph
4. Scheduler เริ่มรัน task ของ ADCS

## ไฟล์หลัก

- `adcs_config.h` เก็บ timing, priority, stack size, gain, limit และ telemetry setting
- `adcs_types.h` เก็บ enum และ struct กลาง เช่น `SensorSample`, `NavigationSolution`, `ActuatorCommand`, `SharedState`
- `adcs_platform.h` เป็น wrapper ระหว่าง ADCS กับ HAL เช่น tick, safe input, UART transmit
- `adcs_app.h` เป็น application layer หลักของ FreeRTOS task graph
- `adcs_controller.h` เลือก control law ตาม mode
- `adcs_magnetic_detumble.h` คำนวณ magnetic dipole command สำหรับ detumble
- `adcs_bdot.h` และ `adcs_rate_dumping.h` เป็น helper ที่มีอยู่ แต่ไม่ได้ถูกใช้เป็น default path ปัจจุบัน

## Task Graph

ระบบมี 5 tasks:

- `ADCS_SENSOR` สร้าง fail-safe placeholder `SensorSample` ทุก `100 ms` จนกว่าจะต่อ sensor source จริง
- `ADCS_NAV` filter body rate และเลือก mode
- `ADCS_CTRL` คำนวณ actuator command ทุก `100 ms` และบันทึกไว้ใน shared state
- `ADCS_TLM` log telemetry snapshot เมื่อมี event หรือทุก `1000 ms`
- `ADCS_HEALTH` ตรวจ sensor link, heartbeat, queue overrun และ safe/fault state ทุก `250 ms`

Queue หลักมี 2 ตัว:

- `sensor_queue`: ส่ง `SensorSample` จาก sensor task ไป navigation task
- `navigation_queue`: ส่ง `NavigationSolution` จาก navigation task ไป control task

Shared state ถูกป้องกันด้วย mutex `state_lock`

## ค่าคงที่สำคัญ

ค่าปัจจุบันอยู่ใน `adcs_config.h`:

- `kSensorTaskPeriodMs = 100`
- `kControlTaskPeriodMs = 100`
- `kTelemetryTaskPeriodMs = 1000`
- `kHealthTaskPeriodMs = 250`
- `kPipelineWatchdogTimeoutMs = 500`
- `kQueueDepth = 32`
- `kStartupModeDurationMs = 0`
- `kDetumbleRateThresholdRadPerSec = 0.05`
- `kDetumbleExitRateThresholdRadPerSec = 0.00349066`
- `kDipoleLimitAm2 = 8.0`
- `kCommandLimitNm = 29.63`

## Mode Logic

โหมดถูกเลือกใน `DetermineMode(...)` ใน `adcs_app.h`

ลำดับ decision ปัจจุบัน:

1. ถ้า `sample.sensor_ok == false` ให้เข้า `Fault`
2. ถ้า `sample.safe_request == true` ให้เข้า `Safe`
3. ถ้า `kStartupModeDurationMs > 0` และเวลาระบบยังไม่เกินค่านี้ ให้เข้า `Startup`
4. ถ้า body-rate norm สูงพอ ให้เข้า `Detumble`
5. ถ้าไม่เข้าเงื่อนไขใด ให้เข้า `Safe`

หมายเหตุสำคัญ:

- `kStartupModeDurationMs` ปัจจุบันเป็น `0` ดังนั้น `Startup` เป็น initial state เท่านั้น
- `SunPoint` มีอยู่ใน enum และ controller แต่ mode selector ปัจจุบันยังไม่ return `SunPoint`
- ถ้า health state มี fault หรือ safe request, `ControlTask` จะ override mode เป็น `Fault` หรือ `Safe` ก่อนคำนวณ command

## Detumble

เมื่อเข้า `Detumble`, controller ใช้ `magnetic_detumble::ComputeDipoleCommand(...)`

แนวคิดคือสร้าง magnetic dipole command จาก body rate และ magnetic field ใน body frame:

```text
dipole_cmd = scale * (body_rate x B_unit)
dipole_cmd = saturate(dipole_cmd, +/- kDipoleLimitAm2)
```

ถ้า magnetic field norm ต่ำกว่า `kMinimumFieldNormTesla`, command จะเป็นศูนย์เพื่อเลี่ยงการหารด้วยค่าที่เล็กเกินไป

## Safe

`Safe` ใช้หยุด actuator command โดย task graph ยังทำงานต่อ ระบบยังอ่าน sensor, update health และส่ง telemetry event ได้

Safe เกิดได้จาก:

- ปุ่ม/user input `B1` ที่อ่านผ่าน `platform::IsSafeRequested()`
- fallback ของ mode selector เมื่อไม่เข้าเงื่อนไข detumble/fault/startup
- health state override ใน `ControlTask`

## Fault

Fault เกิดจาก:

- sensor link stale หรือยังไม่มี valid frame
- queue overrun
- sensor task stale
- navigation task stale
- control task stale
- stack overflow หรือ malloc failed จะเข้า `Error_Handler()`

เมื่อ fault active, mode ถูกยกเป็น `Fault` และ controller ไม่สั่ง actuator command

## UART Telemetry

USART3 ใช้ baud rate `460800` และตอนนี้ใช้สำหรับ bounded text telemetry เท่านั้น
binary simulator protocol เดิมถูกลบออกแล้ว

## จุดที่เหมาะสำหรับพัฒนาต่อ

- เพิ่ม hardware sensor จริงใน path ที่สร้าง `SensorSample`
- เพิ่ม estimator จริงใน `BuildNavigationSolution(...)`
- ปรับ mode selector ให้เข้า `SunPoint` เมื่อ rate ต่ำและ sensor healthy
- ตัดสินใจว่าจะใช้ `magnetic_detumble`, `B-dot`, หรือ `rate_dumping` เป็น detumble path หลัก
- เพิ่ม LED status ใน `HealthTask` ถ้าต้องการ feedback บนบอร์ด
- เพิ่ม unit test ฝั่ง host สำหรับ frame parser, CRC, mode logic และ controller math
