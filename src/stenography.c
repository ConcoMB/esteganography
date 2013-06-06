#include "stenography.h"
#include "hide.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Private methods
void hide_byte(char byte, FILE*p, FILE* out, char alg);
char recover_byte(FILE* in, char alg);
void hide_lsb1(char byte, FILE*p, FILE* out);
char recover_lsb1(FILE* in);
void hide_lsb4(char byte, FILE*p, FILE* out);
char recover_lsb4(FILE* in);
void hide_lsbe(char byte, FILE*p, FILE* out);
char recover_lsbe(FILE* in);


void hide_msg(const char* p_filename, hidden_file_t* file,const char* out_filename, char alg, char has_ext){
	FILE* p = fopen(p_filename,"rb");
	FILE* out = fopen(out_filename,"wb");
	int i;
	for(i=0; i<54; i++){
		fputc(fgetc(p), out);
	}
	// Main
	char c;
	char* ext = file->data+file->size;
	int ext_length = ext[0]=='.' && has_ext ?strlen(ext)+1:0;
	
	// Hide size
	int i_size = invert_int(file->size);
	char* msg = (char*)&i_size;
	for(i=0; i<4; i++){
		hide_byte(msg[i], p, out, alg);
		if(feof(p)){
			printf("Not enough space!");
			exit(-1);
		}
	}
	// Hide data
	msg = file->data;
	for(i=0; i<file->size+ext_length; i++){
		// Hide byte
		hide_byte(msg[i], p, out, alg);
	}
	// Copy the rest
	while(((c=fgetc(p))||1) && !feof(p)){
		fputc(c, out);
	}	
	fclose(p);
	fclose(out);
}

hidden_file_t* recover_msg(const char* filename, char alg, char has_ext){
	FILE* in = fopen(filename,"rb");
	fseek(in, 54, SEEK_SET);
	// Recover
	char *msg;
	hidden_file_t* file = malloc(sizeof(hidden_file_t));
	int size, i;
	for(i=0; i<4; i++){
		*(((char*)&size)+3-i)=recover_byte(in, alg);
	}
	msg = calloc(size+10, sizeof(char));
	for(i=0; i<size; i++){
		msg[i]=recover_byte(in, alg);
	}
	if(has_ext){
		do{
			msg[i]=recover_byte(in, alg);
			i++;
		}while(msg[i-1]);
	}
	file->data=msg;
	file->size=size;
	//
	fclose(in);
	return file;
}


void hide_byte(char byte, FILE*p, FILE* out, char alg){
	switch(alg){
		case LSB1:
			hide_lsb1(byte, p, out);
			break;
		case LSB4:
			hide_lsb4(byte, p, out);
			break;
		default:
			hide_lsbe(byte, p, out);
			break;
	}
}

char recover_byte(FILE* in, char alg){
	switch(alg){
		case LSB1:
			return recover_lsb1(in);
		case LSB4:
			return recover_lsb4(in);
		default:
			return recover_lsbe(in);
	}
}

void hide_lsb1(char byte, FILE*p, FILE* out){
	int i;
	char hidden, c;
	for(i=0;i<8;i++){
		c=fgetc(p);
		hidden = (c & ~1) | get_bit(&byte, i);
		fputc(hidden, out);
	}
}

char recover_lsb1(FILE* in){
	int i;
	char byte=0, hidden;
	for(i=0; i<8; i++){
		hidden = fgetc(in);
		char bit=(hidden&1);
		byte|=(bit<<(7-(i%8)));
	}
	return byte;
}

void hide_lsb4(char byte, FILE*p, FILE* out){
	int i;
	char hidden, c;
	for(i=0;i<2;i++){
		c=fgetc(p);
		hidden = (c & 0xF0) | get_nibble(&byte, i);
		fputc(hidden, out);
	}
}

char recover_lsb4(FILE* in){
	int i;
	char byte=0, hidden;
	for(i=0; i<2; i++){
		hidden = fgetc(in);
		char nibble=(hidden&0x0F);
		byte|=nibble<<(1- i%2)*4;
	}

	return byte;
}

void hide_lsbe(char byte, FILE*p, FILE* out){
	int i;
	char hidden, c, white;
	for(i=0;i<8;i++){
		do{
			c=fgetc(p);
			white = is_white(c);
			if(!white){
				fputc(c,out);
			}
		}while(!white);
		hidden = (c & ~1) | get_bit(&byte, i);
		fputc(hidden, out);
	}
}

char recover_lsbe(FILE* in){
	int i;
	char byte=0, hidden;
	for(i=0; i<8; i++){
		do{
			hidden = fgetc(in);
		}while(!is_white(hidden));
		char bit=(hidden&1);
		byte|=(bit<<(7-(i%8)));
	}
	return byte;
}