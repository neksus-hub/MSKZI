#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

// Функция для вывода 8-битного числа в двоичном формате
void print_bits(uint8_t bits)
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

// Функция для вывода одномерного вектора
void print_vector(const vector<uint8_t>& vect)
{
    for (long unsigned int i = 0; i < vect.size(); i++) {
        cout << hex << setfill('0') << setw(2) << (int)vect[i] << " ";
    }
    cout << endl;
}

// Функция для вывода двумерного вектора
void print_vector(const vector<vector<uint8_t>>& vect)
{
    for (long unsigned int i = 0; i < vect.size(); i++) {
        for (long unsigned int j = 0; j < vect[i].size(); j++)
            cout << hex << setfill('0') << setw(2) << (int)vect[i][j] << " ";
        cout << endl;
    }
}

// Функция для циклического сдвига байта вправо на p бит
uint8_t circle_step(uint8_t byte, int p)
{
    return (byte >> p) | (byte << (8 - p));
}

// Функция для создания таблицы подстановки S2 из таблицы S
vector<vector<uint8_t>> create_S2(const vector<uint8_t>& S)
{
    vector<vector<uint8_t>> S2;
    for (long unsigned int i = 0; i < S.size(); i++) {
        vector<uint8_t> help_vector;
        for (long unsigned int j = 0; j < S.size(); j++) {
            help_vector.push_back(((uint8_t)S[i] << 4) + S[j]);
        }
        S2.push_back(help_vector);
    }
    return S2;
}

// Функция для создания таблицы подстановки S3 из таблицы S2 с использованием циклического сдвига
vector<vector<uint8_t>> create_S3(const vector<vector<uint8_t>>& S)
{
    vector<vector<uint8_t>> S2;
    for (long unsigned int i = 0; i < S.size(); i++) {
        vector<uint8_t> help_vector;
        for (long unsigned int j = 0; j < S[i].size(); j++) {
            help_vector.push_back(circle_step(S[i][j], 5)); // Сдвиг на 5 бит
        }
        S2.push_back(help_vector);
    }
    return S2;
}

int main()
{
    // Значения таблицы S и сдвига p на свои
    vector<uint8_t> S = { 0xC, 0x8, 0x2, 0x1, 0xD, 0x4, 0xF, 0x6, 0x7, 0x0, 0xA, 0x5, 0x3, 0xE, 0x9, 0xB };
    vector<uint8_t> input_16 = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF };
    vector<uint8_t> input_8 = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF };
    vector<uint8_t> output;
    int p = 5; // Значение сдвига
    vector<vector<uint8_t>> S2 = create_S2(S); // Создание таблицы S*
    vector<vector<uint8_t>> S3 = create_S3(S2); // Создание таблицы S'

    // Вывод таблицы подстановки S
    cout << "Tablica zamen S:" << endl;
    print_vector(S);
    cout << endl;

    // Вывод таблицы подстановки S*
    cout << "Tablica zamen S*:" << endl;
    print_vector(S2);
    cout << endl;

    // Вывод таблицы подстановки S'
    cout << "Tablica zamen S':" << endl;
    print_vector(S3);
    cout << endl;

    // 1) Реализация операции подстановки для 4-битного вектора
    cout << "1)Operacia zameni 4-bitnogo vectora:" << endl;
    cout << "   Input vector:  ";
    print_vector(input_16);
    for (long unsigned int i = 0; i < input_16.size(); i++) {
        output.push_back(S[input_16[i]]);
    }
    cout << "   Output vector: ";
    print_vector(output);
    output.clear();
    cout << endl;

    // 2) Реализация подстановки двух смежных 4-битных векторов с использованием таблицы S*
    cout << "2) Zamena dvuh sosednih 4-bitnih vectorov s pomoshiu tablici S*:" << endl;
    cout << "   Input vector:  ";
    print_vector(input_16);
    for (long unsigned int i = 0; i < input_8.size(); i++) {
        output.push_back(S2[input_8[i] & 0x0F][circle_step(input_8[i], p) & 0x0F]);
    }
    cout << "   Output vector: ";
    print_vector(output);
    output.clear();
    cout << endl;

    // 3) Реализация операции перестановки над 8-битным вектором с использованием циклического сдвига
    cout << "3) Operacia perestanovki 8-bitnogo vectora s ispolzovaniem krugovogo sdviga:" << endl;
    cout << "   Input vector:  ";
    print_vector(input_16);
    for (long unsigned int i = 0; i < input_8.size(); i++) {
        output.push_back(input_8[i] & 0x0F);
        output.push_back(circle_step(input_8[i], p) & 0x0F);
    }
    cout << "   Output vector: ";
    print_vector(output);
    output.clear();
    cout << endl;

    // 4) Реализация комбинации операций подстановки и перестановки
    cout << "4) Sochetanie operacii zameni i perestanovki :" << endl;
    cout << "   Input vector:  ";
    print_vector(input_16);
    for (long unsigned int i = 0; i < input_8.size(); i++) {
        output.push_back(S[input_8[i] & 0x0F]);
        output.push_back(S[circle_step(input_8[i], p) & 0x0F]);
    }
    cout << "   Output vector: ";
    print_vector(output);
    output.clear();
    cout << endl;

    // 5) Преобразование файла с использованием преобразования из п. 4
    cout << "5) Transformacia faila s pomoshiu transformacii iz punkta 4:" << endl;
    cout << "   chtenie iz '1.txt', zapis v '2.txt'" << endl;
    ifstream file_1("1.txt", ios::binary | ios::in);
    ofstream file_2("2.txt", ios::binary | ios::out);
    if (!file_1.is_open() || !file_2.is_open()) {
        cerr << "   Error open file!" << endl;
    }
    else {
        char byte_out;
        while (file_1.get(byte_out)) {
            uint8_t byte_in;
            byte_in = (S[(byte_out & 0x0F)] << p) | S[(circle_step(byte_out, p) & 0x0F)];
            file_2.write((char*)&byte_in, sizeof(char));
        }
        cout << "   Preobrazovanie successful." << endl;
    }
    file_1.close();
    file_2.close();
    cout << endl;

    // 6) Реализация комбинации операций подстановки и перестановки с использованием эквивалентной подстановки S'
    cout << "6) Sochetanie zameni i perestanovki s ispolzovaniem ekvivalentnoi zameni S':" << endl;
    cout << "   Input vector:  ";
    print_vector(input_16);
    for (long unsigned int i = 0; i < input_8.size(); i++) {
        output.push_back(S3[input_8[i] & 0x0F][circle_step(input_8[i], p) & 0x0F]);
    }
    cout << "   Output vector: ";
    print_vector(output);
    output.clear();
    cout << endl;

    // 7) Преобразование файла с использованием эквивалентной подстановки из п. 6
    cout << "7) Preobrazovanie faila s ispolzovaniem ekvivalentnoi zameni iz punkta 6:" << endl;
    cout << "   Read from '3.txt', zapis v '4.txt'" << endl;
    ifstream file_3("3.txt", ios::binary | ios::in);
    ofstream file_4("4.txt", ios::binary | ios::out);
    if (!file_3.is_open() || !file_4.is_open()) {
        cerr << "   Error opening file!" << endl;
    }
    else {
        char byte_out;
        while (file_3.get(byte_out)) {
            uint8_t byte_in;
            byte_in = S3[byte_out & 0x0F][circle_step(byte_out, p) & 0x0F];
            file_4.write((char*)&byte_in, sizeof(char));
        }
        cout << "   Preobrazovanie faila zaversheno." << endl;
    }
    file_3.close();
    file_4.close();
    cout << endl;

    return 0;
}