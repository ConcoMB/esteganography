#include "crypto.h"
#include "string.h"
#include "utils.h"
#include "stenography.h"
#include <openssl/evp.h>

#define AES_128 "aes128"
#define AES_192 "aes192"
#define AES_256 "aes256"
#define DES "des"
#define CFB "cfb"
#define CBC "cbc"
#define OFB "ofb"
#define ECB "ecb"

const EVP_CIPHER * get_evp_algorithm(const char*alg, const char * mode);

hidden_file_t* encrypt(hidden_file_t* msg, const char* alg, const char* mode, const char* pass){
  printf("Encrypting with alg:%s, mode: %s, pass: %s\n",alg, mode, pass);
  const EVP_CIPHER *cipher = get_evp_algorithm(alg,mode);
  unsigned char *key= malloc(sizeof(unsigned char) * EVP_CIPHER_key_length(cipher));
  unsigned char *iv= malloc(sizeof(unsigned char) * EVP_CIPHER_iv_length(cipher));
  EVP_BytesToKey(cipher, EVP_md5(), NULL, pass, strlen(pass),1, key, iv);

  hidden_file_t* out = malloc(sizeof(hidden_file_t));
  out->data = calloc(msg->size+16+1,sizeof(char));
  int out_partial_size = 0;
  out->size = 0;
  int i_size = invert_int(msg->size);
  int ext_len = strlen(get_message_extension(msg));
  int total_length = 4+msg->size+ext_len+1;
  char *text = malloc(total_length);
  memcpy(text, &i_size, 4);
  memcpy(text+4, msg->data, msg->size + ext_len + 1);

  EVP_CIPHER_CTX ctx;
  EVP_CIPHER_CTX_init(&ctx);
  EVP_EncryptInit_ex(&ctx, cipher, NULL, key,iv);
  EVP_EncryptUpdate(&ctx, out->data, &out_partial_size, text, total_length);
  EVP_EncryptFinal_ex(&ctx, out->data+out_partial_size, &out->size);

  EVP_CIPHER_CTX_cleanup(&ctx);
  out->size+=out_partial_size;
  return out;
}

hidden_file_t* decrypt(hidden_file_t* msg, const char* alg, const char* mode, const char* pass){
  printf("Decrypting with alg:%s, mode: %s, pass: %s\n",alg, mode, pass);
  const EVP_CIPHER *cipher = get_evp_algorithm(alg,mode);
  unsigned char *key= malloc(sizeof(unsigned char) * EVP_CIPHER_key_length(cipher));
  unsigned char *iv= malloc(sizeof(unsigned char) * EVP_CIPHER_iv_length(cipher));
  EVP_BytesToKey(cipher, EVP_md5(), NULL, pass, strlen(pass),1, key, iv);

  hidden_file_t* out = malloc(sizeof(hidden_file_t));
  out->data = calloc(msg->size+16+1,sizeof(char));
  int out_partial_size = 0;
  out->size = 0;

  EVP_CIPHER_CTX ctx;
  EVP_CIPHER_CTX_init(&ctx);
  EVP_DecryptInit_ex(&ctx, cipher, NULL, key,iv);
  EVP_DecryptUpdate(&ctx, out->data, &out_partial_size, msg->data, msg->size);
  EVP_DecryptFinal_ex(&ctx, out->data+out_partial_size, &out->size);

  EVP_CIPHER_CTX_cleanup(&ctx);
  out->size = invert_int(*((int*)out->data));
  out->data+=4;
  return out;
}

const EVP_CIPHER * get_evp_algorithm(const char*alg, const char * mode){
  if(strcmp(alg, AES_128)==0){
    if(strcmp(mode, ECB)==0){
      return EVP_aes_128_ecb();
    }else if(strcmp(mode, CFB)==0){
      return EVP_aes_128_cfb();
    }else if(strcmp(mode, OFB)==0){
      return EVP_aes_128_ofb();
    }else if(strcmp(mode, CBC)==0){
      return EVP_aes_128_cbc();
    }
  }else if(strcmp(alg, AES_192)==0){
    if(strcmp(mode, ECB)==0){
      return EVP_aes_192_ecb();
    }else if(strcmp(mode, CFB)==0){
      return EVP_aes_192_cfb();
    }else if(strcmp(mode, OFB)==0){
      return EVP_aes_192_ofb();
    }else if(strcmp(mode, CBC)==0){
      return EVP_aes_192_cbc();
    }
  }else if(strcmp(alg, AES_256)==0){
    if(strcmp(mode, ECB)==0){
      return EVP_aes_256_ecb();
    }else if(strcmp(mode, CFB)==0){
      return EVP_aes_256_cfb();
    }else if(strcmp(mode, OFB)==0){
      return EVP_aes_256_ofb();
    }else if(strcmp(mode, CBC)==0){
      return EVP_aes_256_cbc();
    }
  }else if(strcmp(alg, DES)==0){
    if(strcmp(mode, ECB)==0){
      return EVP_des_ecb();
    }else if(strcmp(mode, CFB)==0){
      return EVP_des_cfb();
    }else if(strcmp(mode, OFB)==0){
      return EVP_des_ofb();
    }else if(strcmp(mode, CBC)==0){
      return EVP_des_cbc();
    }
  }
  return NULL;
}