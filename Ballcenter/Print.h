#include <stdint.h>
#include <iostream>

void print_hex(uint8_t * s, size_t len) {

	for (int i = 0; i < len; i++)
		printf("0x%02x, ", s[i]);
	printf("\n");
}