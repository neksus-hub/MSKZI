#ifndef GOST_MAGMA_H
#define GOST_MAGMA_H

#include <vector>
#include <cstdint>

void encryptBlock(const uint8_t* block, uint8_t* out, const std::vector<uint8_t>& key);
void decryptBlock(const uint8_t* block, uint8_t* out, const std::vector<uint8_t>& key);

#endif // GOST_MAGMA_H