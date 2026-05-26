#include "app_modules_config.h"
#if CSP_ENABLE_REGISTER

// cli_commands_register.c
//
// CLI bindings for the Register Service, implemented with the same
// TSC_CMD_EXPORT / TSC_CMD_EXPORT_ALIAS mechanism used by other modules.
//
// Commands:
//   reg_add <dev> <reg> <sub> <type:u8|i8|u16|i16|u32|i32|float|double> <access:ro|wo|rw> <value>
//   reg_set <dev> <reg> <sub> <type> <value>
//   reg_get <dev> <reg> <sub> <type>
//
// Notes:
//   - Data type codes expected by the service are: U8=1, I8=2, U16=3, I16=4, U32=5, I32=6, FLOAT=7, DOUBLE=8.
//   - Access mode maps to the server enum (register_management.h): RO=1, WO=2, RW=3.
//   - The client code already supports loopback when device_id == csp_get_address().

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#include <csp/csp.h>
#include <csp/arch/csp_time.h>

#include "csp_register_client.h"   // add_register_csp / set_register_csp / get_register_csp
#include "register_management.h"   // AccessMode enum (READ_ONLY/WRITE_ONLY/READ_WRITE)
#include <kservice.h>

#ifdef TSC_USING_FINSH
#include "finsh.h"
#endif

static int __reg_add_cmd(int argc, char **argv);
static int __reg_set_cmd(int argc, char **argv);
static int __reg_get_cmd(int argc, char **argv);
static int __reg_rm_cmd(int argc, char **argv);
static int __reg_count_cmd(int argc, char **argv);
static int __reg_status_cmd(int argc, char **argv);

// ---- Helpers: parsing ----

static int parse_type(const char *s)
{
    if (!s)
        return -1;
    char buf[16];
    size_t n = strlen(s);
    if (n >= sizeof(buf))
        return -1;
    for (size_t i = 0; i < n; i++)
        buf[i] = (char)tolower((unsigned char)s[i]);
    buf[n] = '\0';

    if (!strcmp(buf, "u8"))
        return 1;
    if (!strcmp(buf, "i8"))
        return 2;
    if (!strcmp(buf, "u16"))
        return 3;
    if (!strcmp(buf, "i16"))
        return 4;
    if (!strcmp(buf, "u32"))
        return 5;
    if (!strcmp(buf, "i32"))
        return 6;
    if (!strcmp(buf, "float"))
        return 7;
    if (!strcmp(buf, "double"))
        return 8;
    return -1;
}

static int parse_access(const char *s)
{
    if (!s)
        return -1;
    char buf[16];
    size_t n = strlen(s);
    if (n >= sizeof(buf))
        return -1;
    for (size_t i = 0; i < n; i++)
        buf[i] = (char)tolower((unsigned char)s[i]);
    buf[n] = '\0';

    if (!strcmp(buf, "ro"))
        return READ_ONLY; // 1
    if (!strcmp(buf, "wo"))
        return WRITE_ONLY; // 2
    if (!strcmp(buf, "rw"))
        return READ_WRITE; // 3
    return -1;
}

static int parse_uint(const char *s, unsigned *out)
{
    char *end = NULL;
    unsigned long v = strtoul(s, &end, 0);
    if (end == s || *end != '\0')
        return -1;
    *out = (unsigned)v;
    return 0;
}

static void usage_reg_add(void)
{
    printf("Usage: reg_add <dev> <reg> <sub> <type:u8|i8|u16|i16|u32|i32|float|double> <access:ro|wo|rw> <value>\r\n");
}

static void usage_reg_set(void)
{
    printf("Usage: reg_set <dev> <reg> <sub> <type> <value>\r\n");
}

static void usage_reg_get(void)
{
    printf("Usage: reg_get <dev> <reg> <sub> <type>\r\n");
}
static void usage_reg_rm(void)
{
    printf("Usage: reg_rm <dev> <reg> <sub>\r\n");
}
static void usage_reg_count(void)
{
    printf("Usage: reg_count <dev>\r\n");
}
static void usage_reg_status(void)
{
    printf("Usage: reg_status <dev>\r\n");
}

// Convert a string value into a typed payload placed into a 64-bit box.
// Returns 0 on success and sets *sz to the actual payload size.
static int fill_value_by_type(int type, const char *s, uint64_t *box, size_t *sz)
{
    char *end = NULL;
    switch (type)
    {
    case 1:
    { // U8
        unsigned long v = strtoul(s, &end, 0);
        if (end == s || *end != '\0' || v > 0xFF)
            return -1;
        *(uint8_t *)box = (uint8_t)v;
        *sz = sizeof(uint8_t);
        return 0;
    }
    case 2:
    { // I8
        long v = strtol(s, &end, 0);
        if (end == s || *end != '\0' || v < -128 || v > 127)
            return -1;
        *(int8_t *)box = (int8_t)v;
        *sz = sizeof(int8_t);
        return 0;
    }
    case 3:
    { // U16
        unsigned long v = strtoul(s, &end, 0);
        if (end == s || *end != '\0' || v > 0xFFFF)
            return -1;
        *(uint16_t *)box = (uint16_t)v;
        *sz = sizeof(uint16_t);
        return 0;
    }
    case 4:
    { // I16
        long v = strtol(s, &end, 0);
        if (end == s || *end != '\0' || v < -32768 || v > 32767)
            return -1;
        *(int16_t *)box = (int16_t)v;
        *sz = sizeof(int16_t);
        return 0;
    }
    case 5:
    { // U32
        unsigned long v = strtoul(s, &end, 0);
        if (end == s || *end != '\0')
            return -1;
        *(uint32_t *)box = (uint32_t)v;
        *sz = sizeof(uint32_t);
        return 0;
    }
    case 6:
    { // I32
        long v = strtol(s, &end, 0);
        if (end == s || *end != '\0')
            return -1;
        *(int32_t *)box = (int32_t)v;
        *sz = sizeof(int32_t);
        return 0;
    }
    case 7:
    { // FLOAT
        float v = strtof(s, &end);
        if (end == s || *end != '\0')
            return -1;
        memcpy(box, &v, sizeof(float));
        *sz = sizeof(float);
        return 0;
    }
    case 8:
    { // DOUBLE
        double v = strtod(s, &end);
        if (end == s || *end != '\0')
            return -1;
        memcpy(box, &v, sizeof(double));
        *sz = sizeof(double);
        return 0;
    }
    }
    return -1;
}
#ifdef TSC_USING_FINSH
int reg_add(int argc, char **argv) { return __reg_add_cmd(argc, argv); }
int reg_set(int argc, char **argv) { return __reg_set_cmd(argc, argv); }
int reg_get(int argc, char **argv) { return __reg_get_cmd(argc, argv); }
int reg_rm(int argc, char **argv) { return __reg_rm_cmd(argc, argv); }
int reg_count(int argc, char **argv) { return __reg_count_cmd(argc, argv); }
int reg_status(int argc, char **argv) { return __reg_status_cmd(argc, argv); }
// ---- reg_add ----
static int __reg_add_cmd(int argc, char **argv)
{
    if (argc != 7)
    {
        usage_reg_add();
        return -1;
    }

    unsigned dev, reg, sub;
    int type, access;
    if (parse_uint(argv[1], &dev) || parse_uint(argv[2], &reg) || parse_uint(argv[3], &sub))
    {
        usage_reg_add();
        return -1;
    }
    if ((type = parse_type(argv[4])) < 0)
    {
        usage_reg_add();
        return -1;
    }
    if ((access = parse_access(argv[5])) < 0)
    {
        usage_reg_add();
        return -1;
    }

    uint64_t box = 0;
    size_t sz = 0;
    if (fill_value_by_type(type, argv[6], &box, &sz) != 0)
    {
        printf("Invalid value for given type\r\n");
        return -1;
    }

    int ms = add_register_csp(dev, reg, sub, (uint8_t)type, &box, (uint8_t)access, /*timeout*/ 1000);
    if (ms >= 0)
    {
        printf("OK (%d ms)\r\n", ms);
        return 0;
    }
    else
    {
        printf("ERR (%d)\r\n", ms);
        return -1;
    }
}

// ---- reg_set ----
static int __reg_set_cmd(int argc, char **argv)
{
    if (argc != 6)
    {
        usage_reg_set();
        return -1;
    }

    unsigned dev, reg, sub;
    int type;
    if (parse_uint(argv[1], &dev) || parse_uint(argv[2], &reg) || parse_uint(argv[3], &sub))
    {
        usage_reg_set();
        return -1;
    }
    if ((type = parse_type(argv[4])) < 0)
    {
        usage_reg_set();
        return -1;
    }

    uint64_t box = 0;
    size_t sz = 0;
    if (fill_value_by_type(type, argv[5], &box, &sz) != 0)
    {
        printf("Invalid value for given type\r\n");
        return -1;
    }

    int ms = set_register_csp(dev, reg, sub, (uint8_t)type, &box, /*timeout*/ 1000);
    if (ms >= 0)
    {
        printf("OK (%d ms)\r\n", ms);
        return 0;
    }
    else
    {
        printf("ERR (%d)\r\n", ms);
        return -1;
    }
}

// ---- reg_get ----
static int __reg_get_cmd(int argc, char **argv)
{
    if (argc != 5)
    {
        usage_reg_get();
        return -1;
    }

    unsigned dev, reg, sub;
    int type;
    if (parse_uint(argv[1], &dev) || parse_uint(argv[2], &reg) || parse_uint(argv[3], &sub))
    {
        usage_reg_get();
        return -1;
    }
    if ((type = parse_type(argv[4])) < 0)
    {
        usage_reg_get();
        return -1;
    }

    uint64_t box = 0;
    int ms = get_register_csp(dev, reg, sub, (uint8_t)type, &box, /*timeout*/ 1000);
    if (ms >= 0)
    {
        switch (type)
        {
        case 1:
            printf("U8: %u  (%d ms)\r\n", *(uint8_t *)&box, ms);
            break;
        case 2:
            printf("I8: %d  (%d ms)\r\n", *(int8_t *)&box, ms);
            break;
        case 3:
            printf("U16: %u (%d ms)\r\n", *(uint16_t *)&box, ms);
            break;
        case 4:
            printf("I16: %d (%d ms)\r\n", *(int16_t *)&box, ms);
            break;
        case 5:
            printf("U32: %lu (%d ms)\r\n", (unsigned long)*(uint32_t *)&box, ms);
            break;
        case 6:
            printf("I32: %ld (%d ms)\r\n", (long)*(int32_t *)&box, ms);
            break;
        case 7:
        {
            float f;
            memcpy(&f, &box, sizeof(float));
            printf("F32: %g (%d ms)\r\n", f, ms);
        }
        break;
        case 8:
        {
            double d;
            memcpy(&d, &box, sizeof(double));
            printf("F64: %g (%d ms)\r\n", d, ms);
        }
        break;
        }
        return 0;
    }
    else
    {
        printf("ERR (%d)\r\n", ms);
        return -1;
    }
}

// ---- reg_rm ----
static int __reg_rm_cmd(int argc, char **argv)
{
    if (argc != 4)
    {
        usage_reg_rm();
        return -1;
    }
    unsigned dev, reg, sub;
    if (parse_uint(argv[1], &dev) || parse_uint(argv[2], &reg) || parse_uint(argv[3], &sub))
    {
        usage_reg_rm();
        return -1;
    }

    int ms = remove_register_csp(dev, reg, sub, /*timeout*/ 1000);
    if (ms >= 0)
    {
        printf("OK (%d ms)\r\n", ms);
        return 0;
    }
    printf("ERR (%d)\r\n", ms);
    return -1;
}

// ---- reg_count ----
static int __reg_count_cmd(int argc, char **argv)
{
    if (argc != 2)
    {
        usage_reg_count();
        return -1;
    }
    unsigned dev;
    if (parse_uint(argv[1], &dev))
    {
        usage_reg_count();
        return -1;
    }
    uint32_t count = 0;
    int ms = get_register_count_csp(dev, &count, /*timeout*/ 1000);
    if (ms >= 0)
    {
        printf("COUNT: %lu (%d ms)\r\n", (unsigned long)count, ms);
        return 0;
    }
    printf("ERR (%d)\r\n", ms);
    return -1;
}

// ---- reg_status ----
static int __reg_status_cmd(int argc, char **argv)
{
    if (argc != 2)
    {
        usage_reg_status();
        return -1;
    }
    unsigned dev;
    if (parse_uint(argv[1], &dev))
    {
        usage_reg_status();
        return -1;
    }
    uint32_t used = 0, avail = 0;
    int ms = get_register_status_csp(dev, &used, &avail, /*timeout*/ 1000);
    if (ms >= 0)
    {
        printf("USED: %lu  AVAIL: %lu  (%d ms)\r\n", (unsigned long)used, (unsigned long)avail, ms);
        return 0;
    }
    printf("ERR (%d)\r\n", ms);
    return -1;
}

// ---- Bind to the CLI (same macro style as existing commands) ----
// Adjust the alias strings if you prefer a different naming style.
TSC_CMD_EXPORT_ALIAS(__reg_add_cmd, reg_add,
                     "Add a register: reg_add <dev> <reg> <sub> <type> <access> <value>");
TSC_CMD_EXPORT_ALIAS(__reg_set_cmd, reg_set,
                     "Set a register: reg_set <dev> <reg> <sub> <type> <value>");
TSC_CMD_EXPORT_ALIAS(__reg_get_cmd, reg_get,
                     "Get a register: reg_get <dev> <reg> <sub> <type>");
TSC_CMD_EXPORT_ALIAS(__reg_rm_cmd, reg_rm,
                     "Remove a register: reg_rm <dev> <reg> <sub>");
TSC_CMD_EXPORT_ALIAS(__reg_count_cmd, reg_count,
                     "Get used count: reg_count <dev>");
TSC_CMD_EXPORT_ALIAS(__reg_status_cmd, reg_status,
                     "Get status: reg_status <dev>");
#endif


#endif /* CSP_ENABLE_REGISTER */
