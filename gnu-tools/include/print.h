#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include "types.h"
#include "utils.h"
#include "system.h"
enum {
    PRINT_COLOR_BLACK = 0,
	PRINT_COLOR_BLUE = 1,
	PRINT_COLOR_GREEN = 2,
	PRINT_COLOR_CYAN = 3,
	PRINT_COLOR_RED = 4,
	PRINT_COLOR_MAGENTA = 5,
	PRINT_COLOR_BROWN = 6,
	PRINT_COLOR_LIGHT_GRAY = 7,
	PRINT_COLOR_DARK_GRAY = 8,
	PRINT_COLOR_LIGHT_BLUE = 9,
	PRINT_COLOR_LIGHT_GREEN = 10,
	PRINT_COLOR_LIGHT_CYAN = 11,
	PRINT_COLOR_LIGHT_RED = 12,
	PRINT_COLOR_PINK = 13,
	PRINT_COLOR_YELLOW = 14,
	PRINT_COLOR_WHITE = 15,
};

int cursorX , cursorY;
const uint8 sw ,sh ,sd ; 
                                                    //We define the screen width, height, and depth.
void clearLine(uint8 from,uint8 to);

void updateCursor();

void clearScreen();

void scrollUp(uint8 lineNumber);

void newLineCheck();
void printfch(char c);

void print_clear();
void print_char(char character);
void print_str(char* string);
void print_int(int value);
void print_float(float value); 

void print_set_color(uint8_t foreground, uint8_t background);
void printf(const char* format, ...);