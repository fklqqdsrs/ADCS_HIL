//! @file data_monitor.h

// --------------------------------------------------------------------------------------------------- //
// COMPANY NAME : IOT POW.
//
// INTRODUCTION : Data Monitor main functions.
//
// AUTHOR(S) : Pongpot Chaiboonuang
// CONTACT E-MAIL ADDRESS : pongpot.c@gmail.com
//
// REFERENCE DOCUMENT :
// N/A
//
// NOTE :
// N/A
//
// --------------------------------------------------------------------------------------------------- //

#ifndef DATA_MONITOR_CORE_H
#define DATA_MONITOR_CORE_H

#if defined __cplusplus
extern "C"
{
#endif
// ---------- (1) SYSTEM INCLUDE --------------------------------------------------------------------- //
// N/A

// ---------- (2) EXTERNAL MODULE INCLUDE ------------------------------------------------------------ //
// N/A

// ---------- (3) PUBLIC PROGRAMMING DEFINE ---------------------------------------------------------- //
#define DATA_MONITOR_MAX_DATA_BYTES sizeof(union save_type_u) // max payload bytes per sample (adjust as needed)

#define DATA_MON_ERROR_READ -3
#define DATA_MON_ERROR_TYPE -2
#define DATA_MON_ERROR_LENGTH -1
#define DATA_MON_ERROR_OK 0
#define DATA_MON_ERROR_MAX 1
#define DATA_MON_ERROR_MIN 2

    // ---------- (4) ENUMERATOR DEFINITION -------------------------------------------------------------- //
    // N/A

    // ---------- (5) STRUCT OR UNION DATA TYPE DEFINITION ----------------------------------------------- //
    /* --------------------------- Triple redundant storage --------------------------- */
    typedef struct
    {
        uint16_t fec_copy[3][DATA_MONITOR_MAX_DATA_BYTES]; // encoded per-byte (12-bit in 16-bit container)
        uint32_t crc_copy[3];                              // CRC32 of the original raw bytes
        uint8_t raw_copy[3][DATA_MONITOR_MAX_DATA_BYTES];  // optional: keep raw copies (may be flash-encoded in real system)
        size_t data_len;                                   // number of bytes in payload
    } triple_storage_t;

    /* --------------------------- Data Monitor object --------------------------- */
    /* Data monitor config structure */
    typedef struct data_monitor_config
    {
        const char *name;
        cfg_type_t type;
        config_entry_t *config_range;
        triple_storage_t storage;

        int error_code;

        struct data_monitor_config *config_next;
        
        uint16_t mon_id;
    } data_monitor_config_t;

    typedef int (*dm_init_fn)(void *ctx);
    typedef int (*dm_read_fn)(data_monitor_config_t *dmc, void *out_buf, size_t *out_len);
    typedef void (*dm_error_fn)(data_monitor_config_t *dmc, const char *err_msg, const void *bad_data, size_t len);

    /* Data monitor structure */
    typedef struct
    {
        const char *name;
        dm_init_fn init;
        dm_read_fn read;
        dm_error_fn error;
        void *handler_ctx;
        uint32_t period_ms;

        data_monitor_config_t *config_first;
        uint32_t monitor_count;

        osThreadAttr_t *attr;
        osThreadId_t thread_id;
        osMutexId_t storage_mutex;

        bool running;
    } data_monitor_t;

    // ---------- (6) TYPEDEF DATA TYPE DEFINITION ------------------------------------------------------- //
    // N/A

    // ---------- (7) PUBLIC MACRO DEFINITION ------------------------------------------------------------ //
    // N/A

    // ---------- (8) EXTERN FUNCTION -------------------------------------------------------------------- //
    // N/A

    // ---------- (9) EXTERN VARIABLE -------------------------------------------------------------------- //
    // N/A

    // ---------- (10) CLASS DECLARATION ----------------------------------------------------------------- //
    int data_monitor_init(data_monitor_t *dm, const char *name, dm_init_fn init, dm_read_fn read, dm_error_fn error, uint32_t update_period, osThreadAttr_t *attr, void *ctx);
    int data_monitor_get(const char *dm_name, data_monitor_t **dm);
    int data_monitor_register(data_monitor_t *dm);
    int data_monitor_start(data_monitor_t *dm);
    int data_monitor_stop(data_monitor_t *dm);

    int data_monitor_config_register(data_monitor_t *dm, data_monitor_config_t *cfg);
    int data_monitor_config_get(data_monitor_t *dm, const char *dmc_name, data_monitor_config_t **dmc);

    int data_monitor_get_data(data_monitor_t *dm, const char *config_name, uint8_t *out_buf, size_t *out_len);

    /**
     * @brief Set value of a monitor config (single sample) with full range-check and triple storage update.
     *
     * This helper is used both by the periodic data monitor thread and by
     * remote CSP handlers that want to inject a value explicitly.
     *
     * @param dm   Target data monitor
     * @param dmc  Target config entry within monitor
     * @param raw  Pointer to raw value bytes
     * @param len  Length of raw value in bytes
     *
     * @return 0 on success, negative DATA_MON_ERROR_* on validation error.
     */
    int data_monitor_set_value(data_monitor_t *dm, data_monitor_config_t *dmc, const void *raw, size_t len);

// ---------- (11) END OF CLASS DECLARATION ---------------------------------------------------------- //
#if defined __cplusplus
}
#endif

#endif // #ifndef _DATA_MONITOR_H
