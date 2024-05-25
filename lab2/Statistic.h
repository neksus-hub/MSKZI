#pragma once
#include <iostream>
#include <stdlib.h>
#include <vector>
using namespace std;

class Statistic
{
private:
    int frequency_arr[4][16] = { 0 }; // ������ ��� ���������� ����������� ��������� ���������� ���
    int code_tabl_arr[256] = { 0 }; // ������ ��� ���������� ����������� ��������� �������� ������� �������
    uint8_t bits_for_frequency = 0; // ���������� �������� ������������������ ���������� ���
    vector<bool> bits_for_autocorrelation; // ������ ��� ���������� ��������������
    uint64_t size_bits_sequence = 0;       // ���-�� ��� � ������������������
    uint64_t nums_bytes = 0;               // ���-�� ����

public:
    Statistic();
    void clear_all();             //�������� ��� ����������
    void calc_corellation(int t); //���������� ��������������
    void print_num_prob(int bits_num); // ������� ����������� ��������� ���������� ���
    void update_data(uint8_t x);       // ������� ����������
    void update_code_table(uint8_t byte); // ���������� ����������� ��������� �������� ������� �������
    void print_code_table(); // ����� ����������� ��������� �������� ������� �������
    void update_bytes_size(); // ���������� ���-�� ����
};
