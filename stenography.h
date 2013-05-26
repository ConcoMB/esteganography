#ifndef __ESTENO__
#define __ESTENO__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LSB1 1
#define LSB4 2
#define LSBE 3

void hide_msg(const char* p_filename, char* msg,const char* out_filename, char alg);
char* recover_msg(const char* filename, char alg);
char get_bit(char* bytes, int n);
char get_nibble(char* bytes, int n);
void hide_lsb1(char* msg, FILE*p, FILE* out);
char* recover_lsb1(FILE* in);
void hide_lsb4(char* msg, FILE*p, FILE* out);
char* recover_lsb4(FILE* in);
void hide_lsbe(char* msg, FILE*p, FILE* out);
char* recover_lsbe(FILE* in);

#endif