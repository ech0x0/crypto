#ifndef SHA256_H
#define SHA256_H    

#include <stdint.h>
#include <stddef.h>

//returns 0 for success
//returns 1, 2, 3 for invalid parameters
int sha256(uint8_t* hash, const uint8_t* message, const size_t message_len);

#endif 