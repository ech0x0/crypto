#ifndef SHA256_H
#define SHA256_H    

#include <stdint.h>
#include <stddef.h>

// !! hash must be initialized and have a size of atleast 32 bytes (8 uint32_t's)
// !! message_len must be specified IN BITS
//when message_len isn't a multiple of 8 then the first(most significant) message_len % 8 bits from the final byte will be used
//returns 0 for success
//returns 1, 2, 3 for invalid parameters
int sha256(uint32_t* hash, const uint8_t* message, const size_t message_len);

#endif 