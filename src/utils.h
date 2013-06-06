#ifndef __UTILS__
#define __UTILS__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stenography.h"


int invert_int(int little);
char get_bit(char* bytes, int n);
char get_nibble(char* bytes, int n);
char is_white(char byte);
char* get_message_extension(hidden_file_t* file);

#endif