#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stenography.h"
#include "crypto.h"
#include <math.h>

char* message_to_hide(char* filename);
char *get_filename_ext(char *filename);
int file_size(char* filename);
char* preappend_size(char* msg);

int main(){
	char* msg = message_to_hide("mensaje.txt");
	int size = *((int*)msg);
	printf("Mensaje:%d %s\n",size,msg+4);
	char* IV = "AAAAAAAAAAAAAAAA";
	char *key = "0123456789abcdef";
	int keysize = 16; /* 128 bits */

    char* buffer;
    int buffer_len = ceil(size/16.0)*16;
    buffer = calloc(1, buffer_len);
    memcpy(buffer, msg, buffer_len);

	encrypt(buffer, buffer_len, IV, key, keysize, DES, "cbc"); 
	char* encrypted = preappend_size(buffer);
	printf("Encrypted:%d %s\n",*((int*)encrypted),encrypted+4);
	hide_msg("imagen.bmp", encrypted, "out.bmp", LSB4);
	char* recovered = recover_msg("out.bmp", LSB4);
	printf("Recuperado: %d %s\n",*((int*)recovered),recovered+4);

	int encrypted_size=*((int*)recovered);
	buffer_len = ceil(encrypted_size/16.0)*16;
    buffer = calloc(1, buffer_len);
	memcpy(buffer, recovered+4, buffer_len);
	decrypt(buffer, buffer_len, IV, key, keysize, DES, "cbc"); 
	printf("Decrypted:%s\n",buffer+4);
}

char* message_to_hide(char* filename){
	FILE* in = fopen(filename,"r");
	char* extension = get_filename_ext(filename);
	int size = file_size(filename);
	int length = 4+ size + strlen(extension)+1;
	printf("Size: %d, Extension: %s\n",length,extension);
	char* msg = malloc(length);
	char c, *p=msg, i;
	p+=4;
	memcpy(msg, &length, 4);
	while((c=fgetc(in))!=-1){
		*p = c;
		p++;
	}
	fclose(in);
	strcpy(p,extension);
	return msg;
}

char* preappend_size(char* msg){
	int length = strlen(msg)+4;
	char* out = malloc(length);
	char c, *p=out, i;
	p+=4;
	memcpy(out, &length, 4);
	strcpy(out+4,msg);
	return out;
}

char *get_filename_ext(char *filename) {
    char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot;
}

int file_size(char* filename){
	FILE* f = fopen(filename,"rb");
	fseek(f,0,SEEK_END);
	int size = ftell(f);
	fclose(f);
	return size;
}