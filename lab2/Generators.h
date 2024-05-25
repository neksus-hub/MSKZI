#pragma once
#include <iostream>
#include <random>
#include <stdlib.h>
#include <time.h>
using namespace std;

class Generators
{
private:
    uint32_t bits_1 = 0x0;   //���������� ��� �������� x^23+x^5+1
    uint64_t bits_2_1 = 0x0; //������ ���������� ��� �������� x^124+x^37+1
    uint64_t bits_2_2 = 0x0; //������ ���������� ��� �������� x^124+x^37+1
    uint8_t bits_3 = 0x0;    //���������� ��� �������� x^6+x^1+1
    uint8_t bits_4 = 0x0;    //���������� ��� �������� x^5+x^4+x^2+x^1+1
    uint64_t for_zero_first_bit_64 = 0xFFFFFFFFFFFFFFFE;
    uint32_t for_zero_first_bit_32 = 0xFFFFFFFE;
    uint8_t for_zero_first_bit_8 = 0xFE;
    uint64_t size_bits_sequence = 0; //����� ������������������ ���

public:
    Generators();
    void print_bits(uint8_t bits);  // ����� 8-�� ������� �����
    void print_bits(uint32_t bits); // ����� 32-� ������� �����
    void print_bits(uint64_t bits); // ����� 64-� ������� �����
    void print_bit(uint8_t bits);   // ����� ����
    void gen_bit_generators(); // ���������� ���������� ���������� ����������
    uint8_t next_step_generator_1(); //��������� ���� �������� ����� �� 1 ����������
    uint8_t next_step_generator_2(); //��������� ���� �������� ����� �� 2 ����������
    uint8_t next_step_generator_3(); //��������� ���� �������� ����� �� 3 ����������
    uint8_t next_step_generator_4(); //��������� ���� �������� ����� �� 4 ����������
    uint8_t next_step_generators_1_2(); //��������� ���� �������� ����� �� 1 � 2 ����������
    uint8_t next_step_generators_3_4(); //��������� ���� �������� ����� �� 1 � 2 ����������
    uint64_t get_size_bits_sequence(); //��������� ���-�� ���
};

