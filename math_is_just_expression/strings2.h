#pragma once

#include <conio.h>

namespace str {


	void print(const char* str) {
		for (; *str != 0; str++) {
			_putch(*str);
		}
	}

	void strcpy(const char* src, char* dest, int maxBuffSize) {
		int n = 0;
		for (; *src != 0 && (n + 1 < maxBuffSize); src++, dest++, n++){
			*dest = *src;
		}
		*dest = 0;
	}

	void read(char* buffer, int size) {

		const char* const pEnd = buffer + size;

		for (char c = _getch(); c != 13 && (buffer + 1 < pEnd); c = _getch(), buffer++) {
			_putch(c);
			*buffer = c;
		}
		*buffer = 0;
	}

	

}