#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stenography.h"
#include "crypto.h"
#include "utils.h"
#include <math.h>

hidden_file_t* message_to_hide(const char* filename);
char *get_filename_ext(const char *filename);
int file_size(const char* filename);
char* preappend_size(hidden_file_t* file);

int steg_from_string(const char* string){
	if(strcmp(string,"LSBE")==0){
		return LSBE;
	}else if(strcmp(string,"LSB4")==0){
		return LSB4;
	}else{
		return LSB1;
	}
}

int ext(const char * p, const char * out, const char * steg, const char * a, const char * m, const char * pass){
	hidden_file_t* recovered = recover_msg(p, steg_from_string(steg),!*pass);
	printf("Size: %d\n",recovered->size);

	hidden_file_t* output;
	if(*pass){
		// Desencriptar
		output = decrypt(recovered, a, m, pass);
		printf("Decrypted size:%d\n",output->size);
	}else{
		output = recovered;
	}
	char* ext = get_message_extension(output);
	printf("Extension:%s\n",ext);
	char* filename = malloc(strlen(out)+strlen(ext)+1);
	sprintf(filename,"%s%s",out,ext);
	FILE* out_file=fopen(filename,"w");
	fwrite(output->data, 1 , output->size , out_file );
	fclose(out_file);
	return 1;
}

int emb(const char* in, const char * p, const char * out, const char * steg, const char * a, const char * m, const char * pass){
	char steg_n = steg_from_string(steg);
	int in_size=file_size(in), p_size;
	// Check space
	if(steg_n==LSBE){
		p_size=file_whites(p)/8;
	}else{
		p_size=(file_size(p)-54)/steg_n;
	}
	if(in_size > p_size){
		printf("Not enough space -> IN size:%d, P size:%d\n",in_size,p_size);
		exit(-1);
	}
	// Hide the message
	hidden_file_t* msg = message_to_hide(in);
	if(*pass){
		msg = encrypt(msg, a, m, pass);
		printf("Encrypted:%d\n", msg->size);
	}
	hide_msg(p, msg, out, steg_n, !*pass);
	return 0;
}

hidden_file_t* message_to_hide(const char* filename){
	FILE* in = fopen(filename,"r");
	char* extension = get_filename_ext(filename);
	int size = file_size(filename);
	printf("Size: %d, Extension: %s\n",size,extension);
	char* msg = malloc(size+strlen(extension));
	char c, *p=msg;
	while((c=fgetc(in))!=-1){
		*p = c;
		p++;
	}
	fclose(in);
	strcpy(msg+size,extension);
	hidden_file_t* file=malloc(sizeof(hidden_file_t));
	file->size=size;
	file->data=msg;
	return file;
}

//char* preappend_size(hidden_file_t* file){
//	char* out = malloc(file->size+4);
//	char i, *p=(char*)&file->size;
//	for(i=0; i<4; i++){
//		out[3-i]=*p;
//		p++;
//	}
//	memcpy(out+4,file->data, file->size);
//	return out;
//}

char *get_filename_ext(const char *filename) {
    char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot;
}

int file_size(const char* filename){
	FILE* f = fopen(filename,"rb");
	fseek(f,0,SEEK_END);
	int size = ftell(f);
	fclose(f);
	return size;
}

int file_whites(const char* filename){
	FILE* f = fopen(filename,"rb");
	fseek(f, 54, SEEK_SET);
	char c;
	int whites = 0;
	while(((c=fgetc(f))||1)&&!feof(f)){
		if(is_white(c)){
			whites++;
		}
	}
	return whites;
}