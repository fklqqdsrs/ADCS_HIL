
#ifndef NEWC_REG
#define NEWC_REG
#include <string.h>

typedef struct
{
	uint8_t *header;
	uint8_t len;
	uint8_t is_init;
	uint8_t isupdated;
	uint8_t flash_load;
	uint8_t print_log;
}nreg_stuct_t;

void nreg_init(nreg_stuct_t *reg,uint8_t *header, uint8_t len , uint8_t flash_load=0,uint8_t print_log=0){
	reg->header = header;
	reg->len = len;
	reg->is_init = 1;
	reg->isupdated = 0;
	reg->flash_load = flash_load;
	reg->print_log = print_log;
}

uint8_t* nreg_get_header(nreg_stuct_t *reg){
	return reg->header;
}
	
void nreg_get_bytes(nreg_stuct_t *reg,uint8_t *output)
{
		memcpy(output, reg->header, reg->len);
		//reg->isupdated = 0;
}

void nreg_set_bytes(nreg_stuct_t *reg,uint8_t *input)
{
		memcpy(reg->header, input, reg->len);
		reg->isupdated = 1;
}

uint8_t nreg_get_len(nreg_stuct_t *reg) { return reg->len; }

#endif /* NEWC_REG */
