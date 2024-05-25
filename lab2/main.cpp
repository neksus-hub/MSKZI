#include "Generators.h"
#include "Statistic.h"

#include <fstream>
#include <iostream>
#include <stdlib.h>

using namespace std;
int num_zero = 0, num_one = 0; // переменные для отпределения преобладания
uint8_t mask_byte(uint8_t byte, int n_zero, int n_one, Generators& gen, Statistic& ex_bit,
    Statistic& ex_bit_mask) //маскирование байта
{
    uint8_t response_byte = 0;      // байт на выход
    ex_bit.update_code_table(byte); // подсчёт кол-ва каждого символа
    for (int i = 0; i < 8; i++) {
        uint8_t x = gen.next_step_generators_3_4(); // получение бита обратной связи от двух генераторов
        //		uint8_t x = gen.next_step_generators_3_4();
        ex_bit.update_data((byte >> i) & 0x1); // подсчёт статистических данных для бита без маски
        response_byte <<= 1;
        if ((n_zero != 0) && (n_one != 0)) { // проверка на преобладание
            if (x)
                num_one += 1;
            else
                num_zero += 1;
            if (num_one == n_one) {
                response_byte |= 0x00;
                num_one = 0;
            }
            else if (num_zero == n_zero) {
                num_zero = 0;
                response_byte |= 0x01;
            }
        }
        else {
            response_byte |= x;
        }
    }
    response_byte ^= byte;
    for (int i = 7; i >= 0; i--) {
        ex_bit_mask.update_data((response_byte >> i) & 0x1); // подсчёт статистики для байтов после маскирования
    }
    ex_bit_mask.update_code_table(response_byte); // подсчёт кол-ва каждого символа после маскирования
    return response_byte;
}
int main(int argc, char** argv)
{
    int num_command, bits_size, bits_comb, t, preobl;
    Generators gen;           //определение класса генератора
    gen.gen_bit_generators(); //генерация начальных значений регистра
    cout << "1-generaciya chisla 2-maskirovanie faila" << endl;
    cin >> num_command;
    if (num_command == 1) {
        Statistic stat;
        cout << "Kolichestvo bit: " << endl;
        cin >> bits_size;
        cout << "Kolichestvo bit proverki (1-4): " << endl;
        cin >> bits_comb;
        cout << "Sgenerirovanaya posledovatelnost: " << endl;
        for (int i = 0; i < bits_size; i++) {
            uint8_t x = gen.next_step_generators_1_2(); //генерация бита обратной связи от двух генераторов
            // uint8_t x = gen.next_step_generators_3_4();
            stat.update_data(x); // подсчёт статистики
            gen.print_bit(x);    // вывод бита
        }
        cout << endl;
        stat.print_num_prob(bits_comb); // вывод статистики
        cout << "Avtokorelyaziya PSP:" << endl;
        cout << "Vvedite t: " << endl;
        cin >> t;
        stat.calc_corellation(t); // вывод автокорреляции
    }
    else if (num_command == 2) {
        Statistic stat, stat_mask; // объявление экземляров класса статистики для последовательности бит файла до и
        // после маскирования
        char byte_out;
       // ifstream file_1("/home/user/Projects_C/Project_1/Project_1/Project_1/meow.bmp", ios::binary | ios::in);
       // ofstream file_2("/home/user/Projects_C/Project_1/Project_1/Project_1/meow_mask.bmp", ios::binary | ios::out);
                ifstream file_1("D:/учеба/Универ/МиСКЗИ/Лабораторная работа №2/lab2/1.txt", ios::binary | ios::in);
                ofstream file_2("D:/учеба/Универ/МиСКЗИ/Лабораторная работа №2/lab2/1_mask.txt", ios::binary | ios::out);
        if (!file_1) {
            cout << "file ne naiden" << endl;
            return 1;
        }
        cout << "Preobladanie s shagom 3, 7? (1-Да, Другое число-Нет)" << endl;
        cin >> preobl;
        cout << "Kolichestvo bit proverki? (1-4): " << endl;
        cin >> bits_comb;
        cout << "Vvedite t: " << endl;
        cin >> t;
        cout << "Load..." << endl;
        for (int i = 0; i < 54; i++) {
            file_1.get(byte_out);
            file_2.write((char*)&byte_out, sizeof(char));
        }
        while (file_1.get(byte_out)) {
            uint8_t byte_in;
            stat.update_bytes_size();      //подсчёт байтов
            stat_mask.update_bytes_size(); //подсчёт маскированных байтов
            if (preobl == 1)
                byte_in = mask_byte((uint8_t)byte_out, 3, 7, gen, stat, stat_mask);
            else
                byte_in = mask_byte((uint8_t)byte_out, 0, 0, gen, stat, stat_mask);
            file_2.write((char*)&byte_in, sizeof(char));
        }
        cout << endl;
        cout << "sucses!" << endl;
        cout << "Statistika dlya bitov faila bez maski:" << endl;
        stat.print_num_prob(bits_comb);
        stat.calc_corellation(t);
        cout << endl;
        cout << "Raspredelenie veroyatnostej poyavleniya simvolov kodovoj tablicy do maskirovaniya" << endl;
        stat.print_code_table();
        cout << "Statistika dlya bitov fajla s maskoj:" << endl;
        stat_mask.print_num_prob(bits_comb);
        stat_mask.calc_corellation(t);
        cout << "Raspredelenie veroyatnostej poyavleniya simvolov kodovoj tablicy posle maskirovaniya" << endl;
        stat_mask.print_code_table();
    }
    else {
        cout << "Horoshaia rabota" << endl;
    }
    return 0;
}