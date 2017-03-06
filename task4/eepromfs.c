#include "eepromfs.h"

typedef struct fileTable {
	file_t[] files;
	
}

typedef struct file {
	uint8_t size;
	uint8_t* start;
} file_t;

typedef struct fileblock {
	
	struct fileblock* next;
} fileblock_t;

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
	int* ptr = 
	 uint8_t byte = eeprom_read_byte((uint8_t*) ptr)
}

void delete(uint8_t filename){
	
}