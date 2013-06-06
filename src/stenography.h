#ifndef __ESTENO__
#define __ESTENO__

#define LSB1 8
#define LSB4 2
#define LSBE 1

typedef struct hidden_file {
	int size;
	char* data;
} hidden_file_t;

void hide_msg(const char* p_filename, hidden_file_t* file,const char* out_filename, char alg, char has_ext);
hidden_file_t* recover_msg(const char* filename, char alg, char has_ext);

#endif