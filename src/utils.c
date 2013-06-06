#include "utils.h"

char get_bit(char* bytes, int n){
	 return (bytes[n/8] >> (7-(n%8)))&1;
}

char get_nibble(char* bytes, int n){
	 return (bytes[n/2] >> (4*((n+1)%2)))&0x0F;
}

int invert_int(int little){
	int big;
	char i, *l=(char*)&little, *b=(char*)&big;
	for(i=0; i<4; i++){
		b[3-i]=*l;
		l++;
	}
	return big;
}

char is_white(char byte){
	return byte==(char)0xFF || byte==(char)0xFE;
}

char* get_message_extension(hidden_file_t* file){
	return file->data+file->size;
}