#include <iostream>
#include <stdio.h>
#include <fstream>
using namespace std;

int main()
{
    int short rlz1 = 1761;  // Начальное состояние РЛЗ1
    int short bit_gamma = 0;          // Символ выходной последовательности такте
    int short buffer = 0;             // Переменная для переноса числа в другую ячейку массива
    long long int rlz2[2] = { 17523682145,652783654752146247 };   //Начальное состояние РЛЗ2
    long long int mask = 0;
    long long int data = 0;
    long long int gamma = 0;              // Выходная последовательность бит
    char Name[16] = {};
    char name[16] = {};
    cout << "Enter the ciphertext file name ";
    cin >> name;
    cout << "Введите имя файла c открытым текстом";
    cin >> Name;
    ofstream out(name, ios::binary);
    ifstream in(Name, ios::binary);
    while (in.read((char*)&data, 8))  {
                for (int i = 0; i < 64; i++) {
                    gamma = gamma << 1;
                    bit_gamma = ((((rlz1 >> 22) ^ (rlz1 >> 17)) & 0x1) ^ (((rlz2[0] >> 59) ^ (rlz2[0] >> 22)) & 0x1));
                    gamma = gamma + bit_gamma;
                    rlz1 = rlz1 << 1;
                    rlz1 = rlz1 + (((rlz1 >> 22) ^ (rlz1 >> 17)) & 0x1);   // Запись бита обратной связи
                    buffer = (rlz2[1] >> 63) & 0x1;
                    rlz2[0] = rlz2[0] << 1;
                    rlz2[1] = rlz2[1] << 1;
                    rlz2[0] = rlz2[0] + buffer;
                    rlz2[1] = rlz2[1] + (((rlz2[0] >> 59) ^ (rlz2[0] >> 22)) & 0x1);   // Запись бита обратной связи
                }
                mask = data ^ gamma;
                out.write((char*)&mask, 8);
    }
    in.close();
    out.close();
    return 0;
}