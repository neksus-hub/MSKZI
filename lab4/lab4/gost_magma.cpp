#include "gost_magma.h"
#include <cstring>

// S-блоки для Магмы
const uint8_t S[8][16] = {
    { 12, 4, 6, 2, 10, 5, 11, 9, 14, 8, 13, 7, 0, 3, 15, 1 },
    { 6, 8, 2, 3, 9, 10, 5, 12, 1, 14, 4, 7, 11, 13, 0, 15 },
    { 11, 3, 5, 8, 2, 15, 10, 13, 14, 1, 7, 4, 12, 9, 6, 0 },
    { 12, 8, 2, 1, 13, 4, 15, 6, 7, 0, 10, 5, 3, 14, 9, 11 },
    { 7, 15, 5, 10, 8, 1, 6, 13, 0, 9, 3, 14, 11, 4, 2, 12 },
    { 5, 13, 15, 6, 9, 2, 12, 10, 11, 7, 8, 1, 4, 3, 14, 0 },
    { 8, 14, 2, 5, 6, 9, 1, 12, 15, 4, 11, 0, 13, 10, 3, 7 },
    { 1, 7, 14, 13, 0, 5, 8, 3, 4, 10, 9, 15, 11, 6, 2, 12 }
};

// Функция замены
uint32_t f(uint32_t x, const uint8_t* k) {
    uint32_t result = x + *reinterpret_cast<const uint32_t*>(k);
    uint8_t* bytes = reinterpret_cast<uint8_t*>(&result);
    for (int i = 0; i < 4; ++i) {
        bytes[i] = (S[i * 2][bytes[i] >> 4] << 4) | S[i * 2 + 1][bytes[i] & 0xF];
    }
    return (result << 11) | (result >> (32 - 11));
}

// Функция шифрования блока
void encryptBlock(const uint8_t* block, uint8_t* out, const std::vector<uint8_t>& key) {
    uint32_t n1 = *reinterpret_cast<const uint32_t*>(block);
    uint32_t n2 = *reinterpret_cast<const uint32_t*>(block + 4);
    for (int i = 0; i < 32; ++i) {
        uint32_t temp = n1;
        n1 = n2 ^ f(n1, key.data() + (i % 8) * 4);
        n2 = temp;
    }
    *reinterpret_cast<uint32_t*>(out) = n2;
    *reinterpret_cast<uint32_t*>(out + 4) = n1;
}

// Функция расшифрования блока
void decryptBlock(const uint8_t* block, uint8_t* out, const std::vector<uint8_t>& key) {
    uint32_t n1 = *reinterpret_cast<const uint32_t*>(block);
    uint32_t n2 = *reinterpret_cast<const uint32_t*>(block + 4);
    for (int i = 31; i >= 0; --i) {
        uint32_t temp = n1;
        n1 = n2 ^ f(n1, key.data() + (i % 8) * 4);
        n2 = temp;
    }
    *reinterpret_cast<uint32_t*>(out) = n2;
    *reinterpret_cast<uint32_t*>(out + 4) = n1;
}