/*
 * imu_protocal.c
 *
 *  Created on: Jan 20, 2026
 *      Author: Chinnapop
 */
#include "imu_protocal.h"

void reverse_bytes(uint8_t *src, uint8_t *dst, uint32_t len) {
    while (len >= 4) { // Process 4 bytes at a time
        dst[0] = src[len - 1];
        dst[1] = src[len - 2];
        dst[2] = src[len - 3];
        dst[3] = src[len - 4];
        dst += 4;
        len -= 4;
    }
    while (len--) { // Process remaining bytes
        *dst++ = *src--;
    }
}

uint32_t calculate_crc32(const uint8_t *data, size_t length) {

    // Pre-calculated lookup table for the polynomial 0x04C11DB7
	// Corrected pre-calculated lookup table for the polynomial 0x04C11DB7
	static const uint32_t crc_table[256] = {
	    0x00000000, 0x04C11DB7, 0x09823B6E, 0x0D4326D9, 0x130476DC, 0x17C56B6B, 0x1A864DB2, 0x1E475005,
	    0x2608EDB8, 0x22C9F00F, 0x2F8AD6D6, 0x2B4BCB61, 0x350C9B64, 0x31CD86D3, 0x3C8EA00A, 0x384FBDBD,
	    0x4C11DB70, 0x48D0C6C7, 0x4593E01E, 0x4152FDA9, 0x5F15ADAC, 0x5BD4B01B, 0x569796C2, 0x52568B75,
	    0x6A1936C8, 0x6ED82B7F, 0x639B0DA6, 0x675A1011, 0x791D4014, 0x7DDC5DA3, 0x709F7B7A, 0x745E66CD,
	    0x9823B6E0, 0x9CE2AB57, 0x91A18D8E, 0x95609039, 0x8B27C03C, 0x8FE6DD8B, 0x82A5FB52, 0x8664E6E5,
	    0xBE2B5B58, 0xBAEA46EF, 0xB7A96036, 0xB3687D81, 0xAD2F2D84, 0xA9EE3033, 0xA4AD16EA, 0xA06C0B5D,
	    0xD4326D90, 0xD0F37027, 0xDDB056FE, 0xD9714B49, 0xC7361B4C, 0xC3F706FB, 0xCEB42022, 0xCA753D95,
	    0xF23A8028, 0xF6FB9D9F, 0xFBB8BB46, 0xFF79A6F1, 0xE13EF6F4, 0xE5FFEB43, 0xE8BCCD9A, 0xEC7DD02D,
	    0x34867077, 0x30476DC0, 0x3D044B19, 0x39C556AE, 0x278206AB, 0x23431B1C, 0x2E003DC5, 0x2AC12072,
	    0x128E9DCF, 0x164F8078, 0x1B0CA6A1, 0x1FCDBB16, 0x018AEB13, 0x054BF6A4, 0x0808D07D, 0x0CC9CDCA,
	    0x7897AB07, 0x7C56B6B0, 0x71159069, 0x75D48DDE, 0x6B93DDDB, 0x6F52C06C, 0x6211E6B5, 0x66D0FB02,
	    0x5E9F46BF, 0x5A5E5B08, 0x571D7DD1, 0x53DC6066, 0x4D9B3063, 0x495A2DD4, 0x44190B0D, 0x40D816BA,
	    0xACA5C697, 0xAC64DB20, 0xA127FDF9, 0xA5E6E04E, 0xBBA1B04B, 0xBF60ADFC, 0xB2238B25, 0xB6E29692,
	    0x8EAF2B2F, 0x8A6E3698, 0x872D1041, 0x83EC0DDD, 0x9DAB5DDD, 0x996A406A, 0x942966B3, 0x90E87B04,
	    0xE4B68DCC, 0xE077907B, 0xED34B6A2, 0xE9F5A115, 0xF7B2F110, 0xF373E6A7, 0xFE3C07E0, 0xFA2DD697,
	    0xC2CE60DF, 0xC60F7D68, 0xC14C5B11, 0xC58D46A6, 0xDBCA16A3, 0xD78B0B14, 0xDA482DCD, 0xDE09307A
	};

    // 1. Initialize CRC register with XOR In value
    uint32_t crc = 0xFFFFFFFF;

    // 2. Process each byte of data
    for (size_t i = 0; i < length; ++i) {
        // Since Reflect In is False, we XOR the next byte with the MSB of the CRC.
        uint8_t index = (uint8_t)(crc >> 24) ^ data[i];
        // Shift CRC left by 8 bits and XOR with the pre-calculated table value.
        crc = (crc << 8) ^ crc_table[index];
    }

    // 3. Final XOR is 0, and Reflect Out is False, so no final steps are needed.
    return crc;
}
