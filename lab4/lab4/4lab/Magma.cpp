#include "Magma.h"
Magma::Magma() {}
void Magma::Deploy_key_into_4(uint64_t key_56)
{
    for(int i = 0; i < 4; i++) {
        deployed_key[i] = (key_56 << (8 + (i * 8))) | (key_56 >> (48 - (i * 8)));
    }
}
void Magma::Divide_deployed_key_into_8()
{
    int index = 0;
    for(int i = 0; i < 4; i++) {
        divided_key[index] = (uint32_t)(deployed_key[i] >> 32);
        divided_key[index + 1] = (uint32_t)deployed_key[i];
        index += 2;
    }
}

//void Magma::Print_bits(uint64_t bits) //функция для вывода 8-ми битного числа
//{
//    int N(64);
//    for(int i = N - 1; i >= 0; i--) {
//        if(bits & ((uint64_t)1 << i))
//            cout << "1";
//        else
//            cout << "0";
//    }
//    cout << endl;
//}

void Magma::Next_round(int round_num, bool flag_round)
{
    a0_ = a0;
    a0 = 0;
    uint32_t key = flag_round ? divided_key[round_num % 8] : divided_key[7 - (round_num % 8)];
    uint32_t mod = static_cast<uint32_t>((static_cast<uint64_t>(a0) + static_cast<uint64_t>(key)) %
                                         UINT32_MAX); // сложение блока с раундовым ключом по модулю 2 в 32;
    for(int i = 7; i >= 0; i--) {
        // преобразование t - подставнока
        a0 = (a0 << 4) | S[i][(mod >> (i * 4)) & 0xF];
    }
    a0 = (a0 << 11) | (a0 >> 21); //циклический сдвиг влево на 11 бит
    // сложение по модулю 2
    a0 ^= a1;
    a1 = a0_;
}
uint64_t Magma::EncryptDecrypt_block(uint64_t block, bool encrypt)
{
    a1 = a0 = a0_ = 0;
    a1 |= (block >> 32) & 0xFFFFFFFF;
    a0 |= block & 0xFFFFFFFF;
    for(int round = 0; round < 32; round++) // раундовый цикл
    {
        if(round != 31) // если непоследний раунд
        {
            if(round <= 23) {
                if(encrypt)
                    Magma::Next_round(round, true);
                else
                    Magma::Next_round(round, false);
            } else {
                if(encrypt)
                    Magma::Next_round(round, false);
                else
                    Magma::Next_round(round, true);
            }
        } else {
            if(encrypt)
                Magma::Next_round(round, false);
            else
                Magma::Next_round(round, true);
            block = 0;
            block = ((a0 | block) << 32) | a1;
        }
    }
    return block;
}