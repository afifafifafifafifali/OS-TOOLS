#include "print.h"
#include "utils.h"
#include "system.h"

int cursorX = 0, cursorY = 0;
const uint8 sw = 80,sh = 25,sd = 2; 
int colorc = 0x0F;

const static size_t NUM_COLS = 80;
const static size_t NUM_ROWS = 25;

struct Char {
    uint8_t character;
    uint8_t color;
};

struct Char* buffer = (struct Char*) 0xb8000;
size_t col = 0;
size_t row = 0;
uint8_t color = PRINT_COLOR_WHITE | PRINT_COLOR_BLACK << 4;


void clearLine(uint8 from,uint8 to)
{
        uint16 i = sw * from * sd;
        string vidmem=(string)0xb8000;
        for(i;i<(sw*to*sd);i++)
        {
                vidmem[(i / 2)*2 + 1 ] = color ;
                vidmem[(i / 2)*2 ] = 0;
        }
}


void updateCursor()
{
    unsigned temp;

    temp = cursorY * sw + cursorX-1;                                                      // Position = (y * width) +  x

    outportb(0x3D4, 14);                                                                // CRT Control Register: Select Cursor Location
    outportb(0x3D5, temp >> 8);                                                         // Send the high byte across the bus
    outportb(0x3D4, 15);                                                                // CRT Control Register: Select Send Low byte
    outportb(0x3D5, temp);                                                              // Send the Low byte of the cursor location
}

void scrollUp(uint8 lineNumber)
{
        string vidmem = (string)0xb8000;
        uint16 i = 0;
        clearLine(0,lineNumber-1);                                            //updated
        for (i;i<sw*(sh-1)*2;i++)
        {
                vidmem[i] = vidmem[i+sw*2*lineNumber];
        }
        clearLine(sh-1-lineNumber,sh-1);
        if((cursorY - lineNumber) < 0 ) 
        {
                cursorY = 0;
                cursorX = 0;
        } 
        else 
        {
                cursorY -= lineNumber;
        }
        updateCursor();
}


void newLineCheck()
{
        if(cursorY >=sh-1)
        {
                scrollUp(1);
        }
}


void printfch(char c)
{
        string vidmem = (string)0xb8000;
        switch(c)
        {
                case (0x08):
                        if(cursorX > 0)
                        {
                                cursorX--;
                                vidmem[(cursorY * sw + cursorX)*2] = 0;
                        }
                        break;
                case (0x09):
                        cursorX = (cursorX + 8) & ~(8-1);
                        break;
                case ('\r'):
                        cursorX = 0;
                        break;
                case ('\n'):
                        cursorX = 0;
                        cursorY++;
                        break;
                default:
                        vidmem[((cursorY * sw + cursorX)*2)] = c;
                        vidmem[((cursorY * sw + cursorX)*2 + 1)] = colorc;
                        cursorX++;
                        break;
        }
        if(cursorX >= sw)
        {
                cursorX = 0;
                cursorY++;
        }
        newLineCheck();
        updateCursor();
}

void clear_row(size_t row) {
    struct Char empty = (struct Char) {
        character: ' ',
        color: color,
    };

    for (size_t col = 0; col < NUM_COLS; col++) {
        buffer[col + NUM_COLS * row] = empty;
    }
}

void print_clear() {
    for (size_t i = 0; i < NUM_ROWS; i++) {
        clear_row(i);
    }
}

void print_newline() {
    col = 0;

    if (row < NUM_ROWS - 1) {
        row++;
        return;
    }

    for (size_t row = 1; row < NUM_ROWS; row++) {
        for (size_t col = 0; col < NUM_COLS; col++) {
            struct Char character = buffer[col + NUM_COLS * row];
            buffer[col + NUM_COLS * (row - 1)] = character;
        }
    }

    clear_row(NUM_COLS - 1);
}

void print_char(char character) {
    if (character == '\n') {
        print_newline();
        return;
    }

    if (col > NUM_COLS) {
        print_newline();
    }

    buffer[col + NUM_COLS * row] = (struct Char) {
        character: (uint8_t) character,
        color: color,
    };

    col++;
}

void print_str(char* str) {
    for (size_t i = 0; 1; i++) {
        char character = (uint8_t) str[i];

        if (character == '\0') {
            return;
        }

        print_char(character);
    }


}


void print_int(int value) {
    if (value < 0) {
        print_char('-');
        value = -value;
    }

    char buffer[11];  // Buffer to hold the largest int value (-2147483648) + '\0'
    int pos = 10;
    buffer[pos] = '\0';

    do {
        buffer[--pos] = (value % 10) + '0';
        value /= 10;
    } while (value != 0);

    print_str(&buffer[pos]);
}

void print_float(float value) {
    // Handle negative values
    if (value < 0) {
        print_char('-');
        value = -value;
    }

    // Extract the integer part
    int integer_part = (int)value;
    print_int(integer_part);

    // Print the decimal point
    print_char('.');

    // Extract and print the fractional part
    float fractional_part = value - (float)integer_part;

    // Handle precision up to 6 decimal places
    for (int i = 0; i < 6; i++) {
        fractional_part *= 10;
        int digit = (int)fractional_part;
        print_char(digit + '0');
        fractional_part -= digit;
    }
}

void print_set_color(uint8_t foreground, uint8_t background) {
    color = foreground + (background << 4);
}


void printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    for (size_t i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%' && format[i + 1] != '\0') {
            i++;  // Move past '%'
            switch (format[i]) {
                case 'c': {
                    char c = (char) va_arg(args, int);  // `char` is promoted to `int` in varargs
                    print_char(c);
                    break;
                }
                case 's': {
                    char* s = va_arg(args, char*);
                    print_str(s);
                    break;
                }
                case 'd': {
                    int d = va_arg(args, int);
                    print_int(d);
                    break;
                }
                case 'f': {
                    float f = (float) va_arg(args, double);  // `float` is promoted to `double` in varargs
                    print_float(f);
                    break;
                }
                default:
                    // If an unknown specifier is encountered, just print it as-is
                    print_char('%');
                    print_char(format[i]);
                    break;
            }
        } else {
            // Print any character that's not part of a format specifier
            print_char(format[i]);
        }
    }

    va_end(args);
}