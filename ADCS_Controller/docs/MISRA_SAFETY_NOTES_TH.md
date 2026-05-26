# MISRA / Safety Notes

เอกสารนี้สรุปขอบเขตการปรับโปรเจค ADCS ให้เป็นแนวทาง safety-critical และ MISRA-oriented
สำหรับงานพัฒนาต่อ โปรเจคนี้ยังไม่ถือว่า certified MISRA จนกว่าจะผ่าน static analysis,
review deviation record, และทดสอบ target hardware ตาม process ของทีม

## Scope ที่ปรับแล้ว

- โค้ด application ใน `Core/Inc/adcs` หลีกเลี่ยง dynamic RTOS allocation สำหรับ task,
  queue, mutex และ event flags โดยส่ง static control block และ static storage ให้ CMSIS-RTOS2
- task stack, queue storage, mutex control block และ event flags control block ถูกเก็บใน
  `AppContext` แบบ static lifetime
- คำสั่งควบคุม ADCS ตรวจ `finite`, limit และ input range ก่อนคำนวณ เพื่อกัน NaN/Inf
  และค่าผิดช่วงไหลเข้าสู่ actuator command
- UART telemetry path ใช้ bounded buffer และคัดลอกข้อมูลก่อนส่งให้ HAL
  เพื่อลด pointer cast และกัน buffer overrun
- sensor/control path ตรวจค่า vector ว่าเป็น finite ก่อนใช้ข้อมูลในคำสั่งควบคุม
- telemetry formatting ตรวจผลลัพธ์จาก `snprintf` และไม่ส่งข้อความที่ truncated หรือ format fail

## Residual Risks / Deviation ที่ยังต้องจัดการ

- `Drivers`, `Middlewares`, CMSIS, HAL, FreeRTOS และ Eigen เป็น third-party/vendor code
  ควรแยกเป็น supplier code พร้อม deviation หรือ compliance evidence ของแต่ละตัว
- `configSUPPORT_DYNAMIC_ALLOCATION` ยังเปิดอยู่ เพราะ Cube/FreeRTOS project ยัง compile
  `heap_4.c` และ CMSIS wrapper มี API ที่รองรับ dynamic path ภายใน framework
  โค้ด ADCS ที่ปรับแล้วไม่ได้พึ่ง dynamic allocation ใน runtime object หลัก
- `std::snprintf` ยังถูกใช้ใน telemetry formatting ต้องมี rule decision/deviation หาก profile
  ของทีมไม่อนุญาต C stdio ใน embedded safety build
- floating-point math และ Eigen ต้องกำหนด toolchain assumptions เช่น IEEE-754 float,
  FPU setting, overflow/NaN policy และ numerical tolerance
- ยังไม่มี MISRA static analyzer report ใน repo จึงยังสรุปได้เพียงว่าเป็น MISRA-oriented
  ไม่ใช่ MISRA-compliant/certified

## Recommendation สำหรับขั้นถัดไป

- รัน static analyzer ที่รองรับ MISRA C++ เช่น Polyspace, PC-lint Plus, Coverity หรือ Cppcheck
  พร้อม MISRA addon ตาม rule set ที่ทีมเลือก
- สร้าง deviation log สำหรับ vendor code, HAL callback signatures, FreeRTOS macros,
  CMSIS wrapper casts และ Eigen template code
- เพิ่ม hardware-in-loop test สำหรับ UART frame error, stale sensor timeout, stack overflow,
  queue full และ actuator command saturation
- พิจารณาแยก safety build profile ที่ปิด API dynamic allocation ทั้งหมด หากต้องการบังคับ
  no-heap policy ระดับโปรเจค
