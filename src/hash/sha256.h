#ifndef SHA256_H
#define SHA256_H    

#include <stdint.h>
#include <stddef.h>

void sha256(uint8_t* hash, uint8_t* message, size_t message_len);

#endif 