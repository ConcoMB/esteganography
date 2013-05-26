#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* message_to_hide(char* filename);
char *get_filename_ext(char *filename);
int file_size(char* filename);
void hide_msg(char* p_filename, char* msg,char* out_filename);
char* recover_msg(char* filename);
char get_bit(char* bytes, int n);

int main(){
	char* msg = message_to_hide("mensaje.txt");
	printf("Mensaje:%d %s\n",*((int*)msg),msg+4);
	hide_msg("imagen.bmp", msg, "out.bmp");
	char* recovered = recover_msg("out.bmp");
	printf("Recuperado: %s",recovered);
}

char* message_to_hide(char* filename){
	FILE* in = fopen(filename,"r");
	char* extension = get_filename_ext(filename);
	int size = file_size(filename);
	int length = 4+ size + strlen(extension)+1;
	printf("Size: %d, Extension: %s\n",length,extension);
	char* msg = malloc(length);
	char c, *p=msg, i;
	for(i=0; i<4; i++){
		*p=*(((char*)&length)+i);
		p++;
	}
	while((c=fgetc(in))!=-1){
		*p = c;
		p++;
	}
	fclose(in);
	strcpy(p,extension);
	return msg;
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

void hide_msg(char* p_filename, char* msg,char* out_filename){
	FILE* p = fopen(p_filename,"rb");
	FILE* out = fopen(out_filename,"wb");
	char c, hidden;
	int i;
	for(i=0; i<54; i++){
		fputc(fgetc(p), out);
	}
	i=0;
	int size = *((int*)msg);
	while(((c=fgetc(p))||1) && !feof(p)){
		if(i<size*8){
			hidden = (c & ~1) | get_bit(msg, i);
			fputc(hidden, out);
			i++;	
		}else{
			fputc(c, out);
		}
		
	}	
	fclose(p);
	fclose(out);
}

char get_bit(char* bytes, int n){
	 return (bytes[n/8] >> (7-(n%8)))&1;
}

char* recover_msg(char* filename){
	FILE* in = fopen(filename,"rb");
	char c, hidden, *msg;
	int size=0, i;
	fseek(in, 54, SEEK_SET);
	for(i=0; i<4*8; i++){
		hidden = fgetc(in);
		*(((char*)&size)+i/8)|=((hidden&1)<<7-(i%8));
	}
	msg = calloc(size, sizeof(char));
	for(i=0; i<(size-4)*8; i++){
		hidden = fgetc(in);
		*(msg+i/8)|=((hidden&1)<<7-(i%8));
	}
	msg[i/8]=0;
	fclose(in);
	return msg;
}