#ifndef __CRYPTO__
#define __CRYPTO__

#include "stenography.h"

hidden_file_t* encrypt(hidden_file_t* msg, const char* alg, const char* mode, const char* pass);
hidden_file_t* decrypt(hidden_file_t* msg, const char* alg, const char* mode, const char* pass);

#endif