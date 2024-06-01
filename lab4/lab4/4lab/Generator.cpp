#include "Generator.h"
Generator::Generator() { srand(time(NULL)); }

void Generator::gen_bit_generators() //функция генерации
{
    bits_1 = rand();
    bits_2_1 = ((uint64_t)rand() << 32) | rand();
    bits_2_2 = ((uint64_t)rand() << 32) | rand();
}

uint8_t Generator::next_step_generator_1() //функция сдвига 32-х битного числа
{
    /* С помощью смежного полинома (1+ x^-16 + x^-23) находим выходной бит,
     * далее сдвигаем биты влево на 1 и записываем на первую позицию выходной бит
     */
    uint32_t response_bit = ((bits_1 >> 15) ^ (bits_1 >> 22)) & 0x1;
    bits_1 = (((bits_1 << 1) & for_zero_first_bit_32) | response_bit);
    return response_bit;
}
uint8_t Generator::next_step_generator_2() //функция сдвига 128-х битного числа
{
    /* Смежный полином (1+ x^-87 + x^-124). Для получения выходного бита сдвигаем второй массив
     * с битами на 87-1-64=22 и на 124-1-6=59, далее сдвигаем на 1 влево второй массив и
     * записываем последний бит первого в начало. Далее сдвигаем первый массив на 1 влево и
     * записываем в начало выходной бит.
     */
    uint64_t response_bit = ((bits_2_2 >> 22) ^ (bits_2_2 >> 59)) & 0x1;
    bits_2_2 = (((bits_2_2 << 1) & for_zero_first_bit_64) | (bits_2_1 >> 63));
    bits_2_1 = (((bits_2_1 << 1) & for_zero_first_bit_64) | response_bit);
    return response_bit;
}

uint8_t Generator::gen_byte()
{
    uint8_t response_bits_8 = 0x0;
    for(int i = 0; i < 8; i++) {
        uint8_t x1 = Generator::next_step_generator_1();
        uint8_t x2 = Generator::next_step_generator_2();
        uint8_t x = x1 ^ x2;
        response_bits_8 |= x;
        if(i < 7)
            response_bits_8 <<= (uint8_t) 1;
    }

    return response_bits_8;
}