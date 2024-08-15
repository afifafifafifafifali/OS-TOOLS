#ifndef UTIL_H
#define UTIL_H

#include "types.h"

void memory_copy(char *source, char *dest, int nbytes); // memcpy
void memory_set(uint8 *dest, uint8 val, uint32 len);    // memset
void int_to_ascii(int n, char str[]);  //intteger to ascii
int str_to_int(string ch)  ;           //str to int
void * malloc(int nbytes);      

#endif