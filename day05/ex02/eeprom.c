#include "eeprom.h"


bool safe_eeprom_read(void *dst, size_t offset, size_t length)
{
	uint32_t magic_number = 0;
	eeprom_read_block(&magic_number,(void *) offset, sizeof(uint32_t));
	if (magic_number != MAGIC_NUMBER)
		return false;
	eeprom_read_block(&dst, (void*) offset + sizeof(uint32_t), length);
	return true;
}

bool safe_eeprom_write(void * buffer, size_t offset, size_t length)
{

}
