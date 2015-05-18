#include <stdint.h>
#include <iostream>

void print_hex(uint8_t *s, size_t len, int * count) {

	for (int i = 0; i < len; i++)
	{
		printf("0x%02x, ", s[i]);
	}
	if (*count == 8) {
		printf("\n");
		*count = 0;
	}
	else
	{
		*count = *count + 1;
	}
}