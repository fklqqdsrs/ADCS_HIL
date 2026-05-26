#include "app_modules_config.h"
#if CSP_ENABLE_DATA_MON

//! @file data_monitor.c
//! @brief C Source File Template

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
// * Data Monitoring module for CubeSat using CMSIS-RTOS v2
// * - Periodic sensor reads
// * - Min/Max checks -> error handler
// * - Triple redundant storage + Forward Error Correction (Hamming per byte)
// * - CRC32 for integrity
// Integrate with Config Manager
//
// --------------------------------------------------------------------------------------------------- //

// ---------- I M P O R T A N T   N O T E ------------------------------------------------------------ //
//
// --------------------------------------------------------------------------------------------------- //
// N/A

// ---------- (1) LOCAL PREDEFINE -------------------------------------------------------------------- //
#define __THIS_FILE_NAME__		"data_monitor.c"

// ---------- (2) STANDARD LIBRARY INCLUDE ----------------------------------------------------------- //
#include <kservice.h>
#include <crc32.h>
#include <config_core.h>
#include <cmsis_os2.h>
#include <csp/csp.h>

// ---------- (3) USER INCLUDE ----------------------------------------------------------------------- //
#include <data_monitor_core.h>

#define LOG_TAG		"data_mon"
#define LOG_LVL		LOG_LVL_ASSERT
#include <tsc_log_core.h>
// ---------- (4) EXTERN OBJECT ---------------------------------------------------------------------- //
// N/A

// ---------- (5) INTERFACE FUNCTION ----------------------------------------------------------------- //
// N/A

// ---------- (6) DATA ------------------------------------------------------------------------------- //
// N/A

// ---------- (7) PRIVATE PROGRAMMING DEFINE --------------------------------------------------------- //
#define DATA_MON_NAME_MAX_LEN		CSP_DATA_MONITOR_NAME_LENGTH
#define DATA_MON_MAX_TASK

#define DATA_MON_COMPARE_TYPE(vtype, dtype)						\
		if(len != sizeof(dtype))								\
			return DATA_MON_ERROR_LENGTH;						\
		dtype vtype##_val = *(dtype*)raw;						\
		dtype vtype##_min = *(dtype*)dmc->config_range->min;	\
		dtype vtype##_max = *(dtype*)dmc->config_range->max;	\
		if(vtype##_val > vtype##_max)							\
			return DATA_MON_ERROR_MAX;							\
		else if(vtype##_val < vtype##_min)						\
			return DATA_MON_ERROR_MIN;							\
		else													\
			return DATA_MON_ERROR_OK;

// ---------- (8) PRIVATE TYPEDEF -------------------------------------------------------------------- //
// N/A

// ---------- (9) PRIVATE FUNCTION ------------------------------------------------------------------- //
// N/A

// ---------- (10) PRIVATE DATA ----------------------------------------------------------------------- //
uint8_t data_mon_count = 0;
data_monitor_t *h_data_mon[CSP_DATA_MONITOR_MAX_TASK];

// ---------- (11) PRIVATE MACRO DEFINITION ---------------------------------------------------------- //
// N/A

// ---------- (12) SOURCE FILE IMPLEMENTATION -------------------------------------------------------- //
/* ----------------------------- Hamming (12,8) per-byte FEC -----------------------------
   We'll encode each 8-bit data byte into a 12-bit codeword (stored in 16-bit container).
   Layout (bits indexing starting at 1 for parity positions):
   positions: 1(p1),2(p2),3(d1),4(p3),5(d2),6(d3),7(d4),8(p4),9(d5),10(d6),11(d7),12(d8)
   p1 covers bits with LSB position having bit1 set, etc. Single-bit error correction supported per byte.
-------------------------------------------------------------------------------------------*/

static uint16_t hamming_encode_byte(uint8_t d)
{
    // extract data bits d1..d8 from d (d1 is LSB)
    uint16_t cw = 0;
    // place data bits
    cw |= ((d >> 0) & 1) << 2;  // pos3
    cw |= ((d >> 1) & 1) << 4;  // pos5
    cw |= ((d >> 2) & 1) << 5;  // pos6
    cw |= ((d >> 3) & 1) << 6;  // pos7
    cw |= ((d >> 4) & 1) << 8;  // pos9
    cw |= ((d >> 5) & 1) << 9;  // pos10
    cw |= ((d >> 6) & 1) << 10; // pos11
    cw |= ((d >> 7) & 1) << 11; // pos12
    // compute parity bits
    // p1 covers positions with bit1 (1): 1,3,5,7,9,11
    int p1 = ((cw >> 2)&1) ^ ((cw >> 4)&1) ^ ((cw >> 6)&1) ^ ((cw >> 8)&1) ^ ((cw >> 10)&1);
    // p2 covers positions with bit2 (2): 2,3,6,7,10,11
    int p2 = ((cw >> 2)&1) ^ ((cw >> 5)&1) ^ ((cw >> 6)&1) ^ ((cw >> 9)&1) ^ ((cw >> 10)&1);
    // p3 covers positions with bit4 (4): 4,5,6,7,12
    int p3 = ((cw >> 4)&1) ^ ((cw >> 5)&1) ^ ((cw >> 6)&1) ^ ((cw >> 11)&1);
    // p4 covers positions with bit8 (8): 8,9,10,11,12
    int p4 = ((cw >> 8)&1) ^ ((cw >> 9)&1) ^ ((cw >> 10)&1) ^ ((cw >> 11)&1);
    // place parity bits
    cw |= (p1 & 1) << 0;  // pos1
    cw |= (p2 & 1) << 1;  // pos2
    cw |= (p3 & 1) << 3;  // pos4
    cw |= (p4 & 1) << 7;  // pos8
    return cw;
}

// Decode and correct single-bit errors. Returns number of corrected bits (0 or 1) or -1 if unrecoverable.
static int hamming_decode_byte(uint16_t cw_in, uint8_t *out_byte)
{
    // compute syndrome
    int b1 = (cw_in >> 0) & 1;
    int b2 = (cw_in >> 1) & 1;
    int b3 = (cw_in >> 3) & 1;
    int b4 = (cw_in >> 7) & 1;
    int d1 = (cw_in >> 2) & 1;
    int d2 = (cw_in >> 4) & 1;
    int d3 = (cw_in >> 5) & 1;
    int d4 = (cw_in >> 6) & 1;
    int d5 = (cw_in >> 8) & 1;
    int d6 = (cw_in >> 9) & 1;
    int d7 = (cw_in >> 10) & 1;
    int d8 = (cw_in >> 11) & 1;
    int s1 = b1 ^ d1 ^ d2 ^ d4 ^ d5 ^ d7;
    int s2 = b2 ^ d1 ^ d3 ^ d4 ^ d6 ^ d7;
    int s3 = b3 ^ d2 ^ d3 ^ d4 ^ d8;
    int s4 = b4 ^ d5 ^ d6 ^ d7 ^ d8;
    int syndrome = (s4<<3) | (s3<<2) | (s2<<1) | s1; // 1..12 indicates bit position
    uint16_t cw = cw_in;
    int corrected = 0;
    if (syndrome != 0) {
        if (syndrome >=1 && syndrome <= 12) {
            // flip the bit
            cw ^= (1u << (syndrome - 1));
            corrected = 1;
        } else {
            return -1; // unrecoverable
        }
    }
    // extract data bits
    uint8_t d = 0;
    d |= ((cw >> 2) & 1) << 0;
    d |= ((cw >> 4) & 1) << 1;
    d |= ((cw >> 5) & 1) << 2;
    d |= ((cw >> 6) & 1) << 3;
    d |= ((cw >> 8) & 1) << 4;
    d |= ((cw >> 9) & 1) << 5;
    d |= ((cw >> 10) & 1) << 6;
    d |= ((cw >> 11) & 1) << 7;
    *out_byte = d;
    return corrected;
}

/* --------------------------- Helper: majority / recovery --------------------------- */
// Try to recover a good copy from triple redundancy. Returns 0 if success and fills out_buf.
static int triple_recover(triple_storage_t *ts, uint8_t *out_buf)
{
    // 1) If two CRCs match, take that copy
    int match_index = -1;
    for (int i = 0; i < 3; ++i) {
        for (int j = i+1; j < 3; ++j) {
            if (ts->crc_copy[i] == ts->crc_copy[j]) {
                match_index = i;
                break;
            }
        }
        if (match_index >= 0) break;
    }
    if (match_index >= 0) {
        // use raw_copy[match_index]
        memcpy(out_buf, ts->raw_copy[match_index], ts->data_len);
        return 0;
    }
    // 2) No two CRCs match. Attempt Hamming decode on each copy and compute CRC of decoded.
    uint8_t decoded_copies[3][DATA_MONITOR_MAX_DATA_BYTES];
    bool decoded_ok[3] = {0};
    for (int k = 0; k < 3; ++k) {
        int unrecoverable = 0;
        for (size_t b = 0; b < ts->data_len; ++b) {
            uint16_t cw = ts->fec_copy[k][b];
            uint8_t dec;
            int res = hamming_decode_byte(cw, &dec);
            if (res < -0) {
                unrecoverable = 1; break;
            }
            decoded_copies[k][b] = dec;
        }
        if (!unrecoverable) {
            uint32_t crc = crc32_compute(decoded_copies[k], ts->data_len);
            if (crc == ts->crc_copy[k]) {
                decoded_ok[k] = true;
            }
        }
    }
    // If any decoded_ok true, choose majority among decoded_ok results by comparing byte-wise.
    for (int k = 0; k < 3; ++k) {
        if (!decoded_ok[k]) continue;
        // try to see if two decoded_ok are identical
        for (int m = k+1; m < 3; ++m) {
            if (!decoded_ok[m]) continue;
            if (memcmp(decoded_copies[k], decoded_copies[m], ts->data_len) == 0) {
                memcpy(out_buf, decoded_copies[k], ts->data_len);
                return 0;
            }
        }
    }
    // 3) Last attempt: attempt majority-vote per byte across decoded_copies (even if some failed)
    for (size_t b = 0; b < ts->data_len; ++b) {
        // collect three candidates (try raw decode even if CRC mismatch)
        uint8_t cand[3];
        for (int k = 0; k < 3; ++k) {
            uint8_t dec;
            int res = hamming_decode_byte(ts->fec_copy[k][b], &dec);
            if (res >= -1) cand[k] = dec; else cand[k] = 0xFF; // if unrecoverable put sentinel
        }
        // majority vote
        if (cand[0] == cand[1] || cand[0] == cand[2]) out_buf[b] = cand[0];
        else if (cand[1] == cand[2]) out_buf[b] = cand[1];
        else out_buf[b] = cand[0]; // fallback
    }
    // verify CRC
    uint32_t crc_final = crc32_compute(out_buf, ts->data_len);
    // If any copy's crc matches derived, accept it
    for (int k = 0; k < 3; ++k) if (crc_final == ts->crc_copy[k]) return 0;
    // else indicate failure
    return -1;
}

/* --------------------------- Storage write helper --------------------------- */
static void triple_store_write(triple_storage_t *ts, const uint8_t *buf, size_t len)
 {
	if (len > DATA_MONITOR_MAX_DATA_BYTES)
		len = DATA_MONITOR_MAX_DATA_BYTES;
	ts->data_len = len;
	uint32_t crc = crc32_compute(buf, len);
	// For simplicity, create three identical copies and encode them.
	for (int k = 0; k < 3; ++k) {
		memcpy(ts->raw_copy[k], buf, len);
		ts->crc_copy[k] = crc;
		for (size_t b = 0; b < len; ++b) {
			ts->fec_copy[k][b] = hamming_encode_byte(ts->raw_copy[k][b]);
		}
	}
}

/* ------------------------ The Data Monitor range check ------------------------- */
static int data_range_check(data_monitor_config_t *dmc, void *raw, size_t len)
{
	if(dmc->config_range->size != len)
		return DATA_MON_ERROR_LENGTH;

	switch (dmc->config_range->type) {
	case CFG_TYPE_UINT8:
		DATA_MON_COMPARE_TYPE(u8, uint8_t);
		break;
	case CFG_TYPE_UINT16:
		DATA_MON_COMPARE_TYPE(u16, uint16_t);
		break;
	case CFG_TYPE_UINT32:
		DATA_MON_COMPARE_TYPE(u32, uint32_t);
		break;
	case CFG_TYPE_UINT64:
		DATA_MON_COMPARE_TYPE(u64, uint64_t);
		break;
	case CFG_TYPE_INT8:
		DATA_MON_COMPARE_TYPE(i8, int8_t);
		break;
	case CFG_TYPE_INT16:
		DATA_MON_COMPARE_TYPE(i16, int16_t);
		break;
	case CFG_TYPE_INT32:
		DATA_MON_COMPARE_TYPE(i32, int32_t);
		break;
	case CFG_TYPE_INT64:
		DATA_MON_COMPARE_TYPE(i64, int64_t);
		break;
	case CFG_TYPE_FLOAT:
		DATA_MON_COMPARE_TYPE(f, float);
		break;
	case CFG_TYPE_DOUBLE:
		DATA_MON_COMPARE_TYPE(d, double);
		break;
	default:
		LOG_E("[RANGE] Unsupported type.\r\n");
		return DATA_MON_ERROR_TYPE;
	}
}

/* ------------------------ Set a single value with validation & storage ------------------------- */
static int data_monitor_set_value_internal(data_monitor_t *dm, data_monitor_config_t *dmc, const void *raw, size_t len)
{
	if (!dm || !dmc || !raw)
		return DATA_MON_ERROR_TYPE;

	/* Validate payload length vs config type */
	if (len == 0 || len > config_type_size(dmc->type))
		return DATA_MON_ERROR_LENGTH;

	/* Range check */
	int out_of_range = data_range_check(dmc, (void *)raw, len);
	if (out_of_range < 0) {
		dmc->error_code = DATA_MON_ERROR_TYPE;
		dm->error(dmc, "data_type", raw, len);
		return out_of_range;
	} else if (out_of_range > 0) {
		dmc->error_code = out_of_range;
		dm->error(dmc, "out_of_range", raw, len);
	} else {
		dmc->error_code = DATA_MON_ERROR_OK;
	}

	/* Store triple redundant */
	osMutexAcquire(dm->storage_mutex, osWaitForever);
	triple_store_write(&dmc->storage, (const uint8_t *)raw, len);
	osMutexRelease(dm->storage_mutex);

	return DATA_MON_ERROR_OK;
}

/* --------------------------- The Data Monitor thread --------------------------- */
static void data_monitor_thread(void *arg)
{
    data_monitor_t *dm = (data_monitor_t*)arg;
    uint32_t period = dm->period_ms;
    while (dm->running) {
        data_monitor_config_t *dmc = (data_monitor_config_t*)dm->config_first;
		for (int i=0; i < dm->monitor_count; i++) {
			uint8_t max_retry = 5;
			do {
				if (!dmc) {
					LOG_E("[MONITOR] DMC is null pointer!");
					continue;
				}

				// read sensor
				uint8_t raw[DATA_MONITOR_MAX_DATA_BYTES];
				size_t out_len = DATA_MONITOR_MAX_DATA_BYTES;
				int r = dm->read(dmc, raw, &out_len);
				if (r != 0) {
					LOG_E("[MONITOR] DMC name = %s read_failed!", dmc->name);
					dmc->error_code = DATA_MON_ERROR_READ;
					dm->error(dmc, "read_failed", NULL, 0);
					// sleep and continue
					osDelay(period);
					continue;
				}
				/* Validate, range-check and store using common helper */
				int out_of_range = data_monitor_set_value_internal(dm, dmc, raw, out_len);
				if (out_of_range < 0) {
					LOG_E("[MONITOR] DMC name = %s data_type Code = %i!", dmc->name, out_of_range);
					break;
				}

				break;
			} while (--max_retry);

			dmc = dmc->config_next;
		}

        osDelay(period);
    }
    dm->thread_id = NULL;
    osThreadExit();
}

/* --------------------------- Public API --------------------------- */
int data_monitor_init(data_monitor_t *dm, const char *name, dm_init_fn init, dm_read_fn read, dm_error_fn error, uint32_t update_period, osThreadAttr_t *attr, void *ctx) {
	if (!dm || !read || !error)
		return -1;
	tsc_memset(dm, 0, sizeof(*dm));
	dm->name = name;
	dm->init = init;
	dm->read = read;
	dm->error = error;
	dm->handler_ctx = ctx;
	dm->period_ms = update_period;
	if (dm->init) {
		int r = dm->init(dm->handler_ctx);
		if (r != 0) {
			return -2;
		}
	}

	dm->attr = attr;
	dm->storage_mutex = osMutexNew(NULL);
	if (!dm->storage_mutex)
		return -3;

	return 0;
}

int data_monitor_get(const char *dm_name, data_monitor_t **dm) {
	for(int i=0; i<data_mon_count; ++i) {
		if(!h_data_mon[i])
			continue;
		if(tsc_strncmp(h_data_mon[i]->name, dm_name, CSP_DATA_MONITOR_NAME_LENGTH) == 0) {
			*dm = h_data_mon[i];
			return 0;
		}
	}

	*dm = NULL;
	return -1;
}

int data_monitor_set_value(data_monitor_t *dm, data_monitor_config_t *dmc, const void *raw, size_t len)
{
	return data_monitor_set_value_internal(dm, dmc, raw, len);
}

int data_monitor_register(data_monitor_t *dm) {
	data_monitor_t *hdm=NULL;

	if(data_monitor_get(dm->name, &hdm) == 0)
		return -1;

	if(data_mon_count >= CSP_DATA_MONITOR_MAX_TASK)
		return -2;

	h_data_mon[data_mon_count++] = dm;
	return 0;
}

int data_monitor_start(data_monitor_t *dm) {
	if (!dm)
		return -1;
	if (dm->running)
		return 0;
	dm->running = true;

	if(dm->attr)
		dm->thread_id = osThreadNew(data_monitor_thread, dm, dm->attr);
	else {
		const osThreadAttr_t attr = { .name = "DataMonitor", .priority = osPriorityNormal, .stack_size = 2048 };
		dm->thread_id = osThreadNew(data_monitor_thread, dm, &attr);
	}

	if (dm->thread_id == NULL)
		return -2;
	return 0;
}

int data_monitor_stop(data_monitor_t *dm) {
	if (!dm)
		return -1;
	dm->running = false;
	// wait for thread to exit
	for (int i = 0; i < 50 && dm->thread_id != NULL; ++i)
		osDelay(10);
	return 0;
}

int data_monitor_config_register(data_monitor_t *dm, data_monitor_config_t *cfg) {
	if(!cfg->name) {
		LOG_E("Config name cannot be null");
		return -1;
	}

	if((cfg->type == CFG_TYPE_BOOL) && (cfg->type >= CFG_TYPE_STRING)) {
		LOG_E("Config type not support");
		return -2;
	}

	if(!cfg->config_range) {
		LOG_E("Config range cannot be null");
		return -3;
	}

	tsc_memset(&cfg->storage, 0, sizeof(triple_storage_t));
	cfg->error_code = DATA_MON_ERROR_OK;
	cfg->config_next = NULL;

	if(dm->monitor_count == 0) {
		dm->config_first = cfg;
	}
	else {
		uint32_t monitor_count = dm->monitor_count;
		data_monitor_config_t *dmc = dm->config_first;

		for(int i=1; i<monitor_count; ++i) {
			dmc = dmc->config_next;
		}

		dmc->config_next = cfg;
	}

	dm->monitor_count++;
    return 0;
}

int data_monitor_config_get(data_monitor_t *dm, const char *dmc_name, data_monitor_config_t **dmc) {
	data_monitor_config_t *dmc_ptr = dm->config_first;

	for(int i=0; i<dm->monitor_count; ++i) {
		if(!dmc_ptr) {
			dmc_ptr = dmc_ptr->config_next;
			continue;
		}

		if(tsc_strncmp(dmc_ptr->name, dmc_name, CSP_DATA_MONITOR_NAME_LENGTH) == 0) {
			*dmc = dmc_ptr;
			return 0;
		}

		dmc_ptr = dmc_ptr->config_next;
	}

	*dmc = NULL;
	return -1;
}

int data_monitor_get_data(data_monitor_t *dm, const char *config_name, uint8_t *out_buf, size_t *out_len)
{
	if (!dm || !out_buf || !out_len)
		return -1;

	data_monitor_config_t *dmc = (data_monitor_config_t*)dm->config_first;
	for(int i=0; i<dm->monitor_count; ++i) {
		if(!dmc)
			return -2;
		if(tsc_strncmp(dmc->name, config_name, CSP_DATA_MONITOR_NAME_LENGTH) == 0)
			break;
		dmc = dmc->config_next;
	}

	if(!dmc)
		return -2;

	osMutexAcquire(dm->storage_mutex, osWaitForever);
	if (dmc->storage.data_len == 0) {
		osMutexRelease(dm->storage_mutex);
		return -3;
	}
	uint8_t recovered[DATA_MONITOR_MAX_DATA_BYTES];
	int rc = triple_recover(&dmc->storage, recovered);
	if (rc == 0) {
		tsc_memcpy(out_buf, recovered, dmc->storage.data_len);
		*out_len = dmc->storage.data_len;
	}
	osMutexRelease(dm->storage_mutex);
	return rc;
}

/**
 * @brief Print the status of all registered data monitor config in a table format.
 */
void data_monitor_print_status(data_monitor_t *dm) {
	char buff[5][32];
	uint8_t out_data[DATA_MONITOR_MAX_DATA_BYTES];
	size_t out_len;

	tsc_kprintf("***** Data Monitor Status *****\r\n");
	tsc_kprintf("Data Monitor Name = %s\r\n", dm->name);
	tsc_kprintf("Update period = %d\r\n\r\n", dm->period_ms);
	tsc_kprintf("| %-16s | %-5s | %-14s | %-14s | %-14s | %-14s | %-20s |\r\n", "Name", "Type", "DefVal", "Min", "Max", "Current Val", "Error");
	tsc_kprintf("|------------------|-------|----------------|----------------|----------------|----------------|----------------------|\r\n");

	data_monitor_config_t *dmc = dm->config_first;

    for (uint32_t i = 0; i < dm->monitor_count; i++) {
    	if(!dmc)
    		return;

    	config_val_str(dmc->config_range->type, dmc->config_range->value, buff[0]);
    	config_val_str(dmc->config_range->type, dmc->config_range->min, buff[1]);
    	config_val_str(dmc->config_range->type, dmc->config_range->max, buff[2]);
    	if(data_monitor_get_data(dm, dmc->name, out_data, &out_len)) {
    		tsc_sprintf(buff[3], "Error Recovery");
    	}
    	else {
    		if(out_len)
    			config_val_str(dmc->type, out_data, buff[3]);
    		else
    			tsc_sprintf(buff[3], "Error Length");
    	}

		switch (dmc->error_code) {
		case DATA_MON_ERROR_READ:
			tsc_sprintf(buff[4], "Read Error");
			break;
		case DATA_MON_ERROR_TYPE:
			tsc_sprintf(buff[4], "Type Error");
			break;
		case DATA_MON_ERROR_LENGTH:
			tsc_sprintf(buff[4], "Length Error");
			break;
		case DATA_MON_ERROR_MAX:
			tsc_sprintf(buff[4], "Over Limit");
			break;
		case DATA_MON_ERROR_MIN:
			tsc_sprintf(buff[4], "Under Limit");
			break;
		case DATA_MON_ERROR_OK:
			tsc_sprintf(buff[4], "None");
			break;
		default:
			tsc_sprintf(buff[4], "Unknown");
		}

        tsc_kprintf("| %-16s |   %-1d   | %-14s | %-14s | %-14s | %-14s | %-20s |\r\n",
                	dmc->name,
					dmc->type,
					buff[0],
					buff[1],
					buff[2],
					buff[3],
					buff[4]);

        dmc = dmc->config_next;
    }

    tsc_kprintf("\r\n************* End *************\r\n");
}

/**
 * @brief Print the status of data monitor by name.
 */
void data_monitor_status(const char *dm_name) {
	if(!dm_name) {
		for(int i=0; i<data_mon_count; ++i) {
			data_monitor_print_status(h_data_mon[i]);
		}
		return;
	}

	data_monitor_t *dm = NULL;
	if(!data_monitor_get(dm_name, &dm)) {
		data_monitor_print_status(dm);
	}else {
		tsc_kprintf("Config name = %s not found! \r\n", dm_name);
	}
}


/**
 * @brief Print the status of registered data monitor config.
 */
void data_monitor_config_print(data_monitor_t *dm, data_monitor_config_t *dmc) {
	if(!dm)
		return;

	char buff[5][32];
	uint8_t out_data[DATA_MONITOR_MAX_DATA_BYTES];
	size_t out_len;

	config_val_str(dmc->config_range->type, dmc->config_range->value, buff[0]);
	config_val_str(dmc->config_range->type, dmc->config_range->min, buff[1]);
	config_val_str(dmc->config_range->type, dmc->config_range->max, buff[2]);
	if (data_monitor_get_data(dm, dmc->name, out_data, &out_len)) {
		tsc_sprintf(buff[3], "Error Recovery");
	} else {
		if (out_len)
			config_val_str(dmc->type, out_data, buff[3]);
		else
			tsc_sprintf(buff[3], "Error Length");
	}

	switch (dmc->error_code) {
	case DATA_MON_ERROR_READ:
		tsc_sprintf(buff[4], "Read Error");
		break;
	case DATA_MON_ERROR_TYPE:
		tsc_sprintf(buff[4], "Type Error");
		break;
	case DATA_MON_ERROR_LENGTH:
		tsc_sprintf(buff[4], "Length Error");
		break;
	case DATA_MON_ERROR_MAX:
		tsc_sprintf(buff[4], "Over Limit");
		break;
	case DATA_MON_ERROR_MIN:
		tsc_sprintf(buff[4], "Under Limit");
		break;
	case DATA_MON_ERROR_OK:
		tsc_sprintf(buff[4], "None");
		break;
	default:
		tsc_sprintf(buff[4], "Unknown");
	}

	tsc_kprintf("***** Data Config Status *****\r\n");
	tsc_kprintf("Config Name = %s\r\n", dmc->name);
	tsc_kprintf("Config Type = %d\r\n", dmc->type);
	tsc_kprintf("Config DefVal = %s\r\n", buff[0]);
	tsc_kprintf("Config Min = %s\r\n", buff[1]);
	tsc_kprintf("Config Max = %s\r\n", buff[2]);
	tsc_kprintf("Config Val = %s\r\n", buff[3]);
	tsc_kprintf("Config Error = %s\r\n", buff[4]);
    tsc_kprintf("\r\n************* End *************\r\n");
}

/**
 * @brief Print the value of data monitor config by name.
 */
void data_monitor_config_status(const char *dm_name, const char *dmc_name) {
	if(!dm_name)
		return;

	data_monitor_t *dm = NULL;
	if(data_monitor_get(dm_name, &dm)) {
		tsc_kprintf("Data Monitor name = %s not found! \r\n", dm_name);
		return;
	}

	data_monitor_config_t *dmc = NULL;
	if(data_monitor_config_get(dm, dmc_name, &dmc)) {
		tsc_kprintf("Data Monitor Config name = %s not found! \r\n", dmc_name);
		return;
	}

	data_monitor_config_print(dm, dmc);
}

#ifdef TSC_USING_FINSH
#include "finsh.h"
/**
 * @brief Print all registered data monitor.
 */
void data_monitor_list(void) {
	tsc_kprintf("***** Data Monitor List *****\r\n");
	for(int i=0; i<data_mon_count; ++i) {
		tsc_kprintf("[No.%d] = %s\r\n\r\n", i+1, h_data_mon[i]->name);
	}
}

void __data_monitor_status(int argc, char **argv)
{
    if (argc == 1)
    {
        /* No arguments -> print all monitors */
        data_monitor_status(NULL);
    }
    else if (argc == 2)
    {
        /* Pass config_name argument */
        data_monitor_status(argv[1]);
    }
    else
    {
        tsc_kprintf("Usage: data_mon_status [monitor_name]\n");
    }
}

void __data_monitor_config_status(int argc, char **argv)
{
	if (argc == 3)
    {
        /* Pass config_name argument */
		data_monitor_config_status(argv[1], argv[2]);
    }
    else
    {
        tsc_kprintf("Usage: data_mon_config_status <monitor_name> <data_name>\n");
    }
}
/* -------------------------------------------------------------------------- */
/* Command Registration (CLI binding)                                         */
/* -------------------------------------------------------------------------- */
TSC_CMD_EXPORT_ALIAS(data_monitor_list, data_mon_list, Data monitor name list.)
TSC_CMD_EXPORT_ALIAS(__data_monitor_status, data_mon_status, Data monitor status.)
TSC_CMD_EXPORT_ALIAS(__data_monitor_config_status, data_mon_config, Data monitor config status.)
#endif
// ---------- (13) END OF SOURCE FILE IMPLEMENTATION ------------------------------------------------- //
// SOURCE FORMAT REVISION B


#endif /* CSP_ENABLE_DATA_MON */
