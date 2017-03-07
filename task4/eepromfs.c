#include "eepromfs.h"

enum fileModes {NONE, READ, WRITE, APPEND} mode = NONE;
table_t table;

typedef struct table_t {
	file_t[] files;
	
} table_t;

typedef struct file_t {
	uint8_t size;
	uint8_t* start;
} file_t;

typedef struct fileblock {
	
	struct fileblock* next;
} fileblock_t;

void init_eepromfs(void){
	
	table_t table = 
}

void open_for_write(uint8_t filename){
	mode = WRITE;
	uint16_t* fileStart;
	if(table_t.file_t[filename].start==0){
		//Allocate start block
		table_t.file_t[filename].size = 0;
	} else {
		fileStart = (table.files[filename].start*BLOCK_SIZE) + EEPROM_START;
	}

}

void open_for_append(uint8_t filename){
	
}

void open_for_read(uint8_t filename){
	
}

void close(uint8_t filename){
	mode = NONE;
}

void write(uint8_t filename, uint8_t *buff, uint16_t len){
	
}

void read(uint8_t filename,  uint8_t *buff, uint16_t len){
	int* ptr = 
	 uint8_t byte = eeprom_read_byte((uint8_t*) ptr)
}

void delete(uint8_t filename){
	table[filename].start=0;
	table[filename].size=0;
	
}