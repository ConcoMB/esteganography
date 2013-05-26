#include "crypto.h"

int encrypt(
    void* buffer,
    int buffer_len, /* Because the plaintext could include null bytes*/
    char* IV, 
    char* key,
    int key_len,
    char* alg,
    char* chain 
){
  MCRYPT td = mcrypt_module_open(alg, NULL, chain, NULL);
  int blocksize = mcrypt_enc_get_block_size(td);
  if( buffer_len % blocksize != 0 ){return 1;}
 
  mcrypt_generic_init(td, key, key_len, IV);
  mcrypt_generic(td, buffer, buffer_len);
  mcrypt_generic_deinit (td);
  mcrypt_module_close(td);
  
  return 0;
}
 
int decrypt(
    void* buffer,
    int buffer_len,
    char* IV, 
    char* key,
    int key_len,
    char* alg,
    char* chain 
){
  MCRYPT td = mcrypt_module_open(alg, NULL, chain, NULL);
  int blocksize = mcrypt_enc_get_block_size(td);
  if( buffer_len % blocksize != 0 ){return 1;}
  
  mcrypt_generic_init(td, key, key_len, IV);
  mdecrypt_generic(td, buffer, buffer_len);
  mcrypt_generic_deinit (td);
  mcrypt_module_close(td);
  
  return 0;
}