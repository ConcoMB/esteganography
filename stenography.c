#include "stenography.h"

void hide_msg(char* p_filename, char* msg,char* out_filename, char alg){
	FILE* p = fopen(p_filename,"rb");
	FILE* out = fopen(out_filename,"wb");
	int i;
	for(i=0; i<54; i++){
		fputc(fgetc(p), out);
	}
	i=0;
	switch(alg){
		case LSB1:
			hide_lsb1(msg, p, out);
			break;
		case LSB4:
			hide_lsb4(msg, p, out);
			break;
		case LSBE:
			hide_lsbe(msg, p, out);
			break;
	}
	fclose(p);
	fclose(out);
}

char get_bit(char* bytes, int n){
	 return (bytes[n/8] >> (7-(n%8)))&1;
}

char get_nibble(char* bytes, int n){
	 return (bytes[n/2] >> (4*((n+1)%2)))&0x0F;
}

char* recover_msg(char* filename, char alg){
	char* msg;
	FILE* in = fopen(filename,"rb");
	fseek(in, 54, SEEK_SET);
	switch(alg){
		case LSB1:
			msg = recover_lsb1(in);
			break;
		case LSB4:
			msg = recover_lsb4(in);
			break;
		case LSBE:
			msg = recover_lsbe(in);
			break;
	}
	fclose(in);
	return msg;
}

void hide_lsb1(char* msg, FILE*p, FILE* out){
	char c, hidden;
	int i, size = *((int*)msg);
	while(((c=fgetc(p))||1) && !feof(p)){
		if(i<size*8){
			hidden = (c & ~1) | get_bit(msg, i);
			fputc(hidden, out);
			i++;	
		}else{
			fputc(c, out);
		}
		
	}	
}

char* recover_lsb1(FILE* in){
	char c, hidden, *msg;
	int size=0, i;
	for(i=0; i<4*8; i++){
		hidden = fgetc(in);
		*(((char*)&size)+i/8)|=((hidden&1)<<7-(i%8));
	}
	msg = calloc(size+4, sizeof(char));
	memcpy(msg, &size, 4);
	msg+=4;
	for(i=0; i<(size-4)*8; i++){
		hidden = fgetc(in);
		*(msg+i/8)|=((hidden&1)<<7-(i%8));
	}
	msg[i/8]=0;
	return msg-4;
}

void hide_lsb4(char* msg, FILE*p, FILE* out){
	char c, hidden;
	int i, size = *((int*)msg);
	while(((c=fgetc(p))||1) && !feof(p)){
		if(i<size*2){
			hidden = c & 0xF0;
			hidden |= get_nibble(msg,i);
			fputc(hidden, out);
			//printf("%x ",hidden);
			//if(i%2==1){
			//	printf(" (%x)\n",msg[i/2]);
			//}
			i++;	
		}else{
			fputc(c, out);
		}
		
	}	
}

char* recover_lsb4(FILE* in){
	char c, hidden, *msg;
	int size=0, i;
	for(i=0; i<4*2; i++){
		hidden = fgetc(in);
		*(((char*)&size)+i/2)|=((hidden&0x0F)<<(4*((i+1)%2)));
	}
	msg = calloc(size+4, sizeof(char));
	memcpy(msg, &size, 4);
	msg+=4;
	for(i=0; i<(size-4)*2; i++){
		hidden = fgetc(in);
		*(msg+i/2)|=((hidden&0x0F)<<(4*((i+1)%2)));
	}
	msg[i/2]=0;
	return msg-4;
}

void hide_lsbe(char* msg, FILE*p, FILE* out){
	unsigned char c, hidden;
	int i, size = *((int*)msg);
	while(((c=fgetc(p))||1) && !feof(p)){
		if(i<size*8 && (c==254 || c==255)){
			hidden = (c & ~1) | get_bit(msg, i);
			fputc(hidden, out);
			i++;	
		}else{
			fputc(c, out);
		}
	}
}

char* recover_lsbe(FILE* in){
	unsigned char c, hidden, *msg;
	int size=0, i=0;
	while(i<4*8){
		hidden = fgetc(in);
		if(hidden==255 || hidden==254){
			*(((char*)&size)+i/8)|=((hidden&1)<<7-(i%8));
			i++;
		}
	}
	msg = calloc(size+4, sizeof(char));
	memcpy(msg, &size, 4);
	msg+=4;
	i=0;
	while(i<(size-4)*8){
		hidden = fgetc(in);
		if(hidden==255 || hidden==254){
			*(msg+i/8)|=((hidden&1)<<7-(i%8));
			i++;
		}
	}
	msg[i/8]=0;
	return msg-4;
}