#include "Generators.h"
Generators::Generators() { srand(time(NULL)); }

void Generators::print_bit(uint8_t bits) //������� ��� ������ ������ ����
{
    if (bits)
        cout << "1";
    else
        cout << "0";
}
void Generators::print_bits(uint8_t bits) //������� ��� ������ 8-�� ������� �����
{
    int N(8);
    for (int i = N - 1; i >= 0; i--) {
        if (bits & ((uint8_t)1 << i))
            cout << "1";
        else
            cout << "0";
    }
    cout << endl;
}
void Generators::print_bits(uint32_t bits) //������� ��� ������ 32-� ������� �����
{
    int N(32);
    for (int i = N - 1; i >= 0; i--) {
        if (bits & ((uint32_t)1 << i))
            cout << "1";
        else
            cout << "0";
    }
    cout << endl;
}

void Generators::print_bits(uint64_t bits) //������� ��� ������ 64-� ������� �����
{
    int N(64);
    for (int i = N - 1; i >= 0; i--) {
        if (bits & ((uint64_t)1 << i))
            cout << "1";
        else
            cout << "0";
    }
    cout << endl;
}
void Generators::gen_bit_generators() //������� ���������
{
    bits_1 = rand();
    bits_2_1 = ((uint64_t)rand() << 32) | rand();
    bits_2_2 = ((uint64_t)rand() << 32) | rand();
    bits_3 = (uint8_t)rand();
    bits_4 = (uint8_t)rand();
}

uint8_t Generators::next_step_generator_1() //������� ������ 32-� ������� �����
{
    /* � ������� �������� �������� (1+ x^-16 + x^-23) ������� �������� ���,
     * ����� �������� ���� ����� �� 1 � ���������� �� ������ ������� �������� ���
     */
    uint32_t response_bit = ((bits_1 >> 15) ^ (bits_1 >> 22)) & 0x1;
    bits_1 = (((bits_1 << 1) & for_zero_first_bit_32) | response_bit);
    return response_bit;
}
uint8_t Generators::next_step_generator_2() //������� ������ 128-� ������� �����
{
    /* ������� ������� (1+ x^-87 + x^-124). ��� ��������� ��������� ���� �������� ������ ������
     * � ������ �� 87-1-64=22 � �� 124-1-6=59, ����� �������� �� 1 ����� ������ ������ �
     * ���������� ��������� ��� ������� � ������. ����� �������� ������ ������ �� 1 ����� �
     * ���������� � ������ �������� ���.
     */
    uint64_t response_bit = ((bits_2_2 >> 22) ^ (bits_2_2 >> 59)) & 0x1;
    bits_2_2 = (((bits_2_2 << 1) & for_zero_first_bit_64) | (bits_2_1 >> 63));
    bits_2_1 = (((bits_2_1 << 1) & for_zero_first_bit_64) | response_bit);
    return response_bit;
}

uint8_t Generators::next_step_generator_3()
{
    uint8_t response_bit = ((bits_3 >> 4) ^ (bits_3 >> 5)) & 0x1;
    bits_3 = (((bits_3 << 1) & for_zero_first_bit_8) | response_bit);
    return response_bit;
}
uint8_t Generators::next_step_generator_4()
{
    uint8_t response_bit = ((bits_4 >> 0) ^ (bits_4 >> 3) ^ (bits_4 >> 4) ^ (bits_4 >> 5)) & 0x1;
    bits_4 = (((bits_4 << 1) & for_zero_first_bit_8) | response_bit);
    return response_bit;
}
uint8_t Generators::next_step_generators_1_2()
{
    uint8_t x1 = Generators::next_step_generator_1();
    uint8_t x2 = Generators::next_step_generator_2();
    size_bits_sequence += 1;
    uint8_t x = x1 ^ x2;
    return x;
}
uint8_t Generators::next_step_generators_3_4()
{
    uint8_t x1 = Generators::next_step_generator_3();
    uint8_t x2 = Generators::next_step_generator_4();
    size_bits_sequence += 1;
    uint8_t x = x1 ^ x2;
    return x;
}
uint64_t Generators::get_size_bits_sequence() { return size_bits_sequence; }