#pragma once
#include <iostream>
#include <random>
#include <stdlib.h>
#include <time.h>
using namespace std;

class Generator
{
private:
    uint32_t bits_1 = 0x0;   //переменная для полинома x^23+x^5+1
    uint64_t bits_2_1 = 0x0; //первая переменная для полинома x^124+x^37+1
    uint64_t bits_2_2 = 0x0; //вторая переменная для полинома x^124+x^37+1
    uint64_t for_zero_first_bit_64 = 0xFFFFFFFFFFFFFFFE;
    uint32_t for_zero_first_bit_32 = 0xFFFFFFFE;
    uint8_t for_zero_first_bit_8 = 0xFE;

public:
    Generator();
    void gen_bit_generators(); // наполнение переменных начальными значениями
    uint8_t next_step_generator_1(); //получение бита обратной связи от 1 генератора
    uint8_t next_step_generator_2(); //получение бита обратной связи от 2 генератора
    uint8_t gen_byte();
};