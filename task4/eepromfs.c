#include "eepromfs.h"

void init_eepromfs(void){
	
}

void open_for_write(uint8_t filename){
	uint16_t fileStart = (BLOCK_SIZE * fileName) + EEPROM_START;
	
}

void open_for_append(uint8_t filename){
	
}

void open_for_read(uint8_t filename){
	
}

void close(uint8_t filename){
	
}

void write(uint8_t filename, uint8_t *buff, uint16_t len){
	
}

void read(uint8_t filename,  uint8_t *buff, uint16_t len){
	
}

void delete(uint8_t filename){
	int i = 0;
	for(i=0; i<BLOCK_SIZE; i++){
		eeprom_update_byte(fileStart+(8*i), 0x00);
	}
}