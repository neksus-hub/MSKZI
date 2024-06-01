#include "Generator.h"
#include "Magma.h"

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
using namespace std;

void gen_key(string file_name)
{
    Generator gen;
    ofstream file_key(file_name + ".key", ios::binary | ios::out);
    gen.gen_bit_generators();
    for(int i = 0; i < 7; i++) {
        uint8_t g = gen.gen_byte();
        file_key.write((char*)&g, sizeof(char));
    }
    file_key.close();
}
uint64_t read_key(string file_name)
{
    ifstream file_key(file_name + ".key", ios::binary | ios::in);
    uint64_t key = 0;
    char byte_out;
    for(int i = 0; i < 7; i++) {
        file_key.get(byte_out);
        key <<= 8;
        key |= (uint64_t)byte_out & 0xFF;
    }
    file_key.close();
    return key;
}
int encrypt_file()
{
    uint64_t key = read_key("key");

    ifstream forCryptFile("For_crypt.txt", ios::binary);
    forCryptFile.seekg(0, ios::end);
    int sizeCryptFileInBytes = forCryptFile.tellg();
    forCryptFile.seekg(0, ios::beg);
    int countBlock = 0;
    const int sizeBlockInbytes = 8;
    ofstream resultFile("Result_crypt.enc", ios::binary);

    Magma mag;
    mag.Deploy_key_into_4(key);
    mag.Divide_deployed_key_into_8();

    uint64_t blockData;
    while(true) {
        countBlock++;

        if(countBlock != (sizeCryptFileInBytes / sizeBlockInbytes) + 1 ||
           sizeCryptFileInBytes % sizeBlockInbytes ==
               0) // если читается не последний блок данных из файла или размер файла кратен размеру блоку
        {
            forCryptFile.read((char*)&blockData, sizeBlockInbytes);
            if(forCryptFile.eof()) {
                break;
            }
        } else {

            forCryptFile.read((char*)&blockData, sizeCryptFileInBytes % sizeBlockInbytes);
            if(forCryptFile.eof()) {
                break;
            }
            uint32_t countBits = (sizeCryptFileInBytes % sizeBlockInbytes) * 8;
            blockData = (blockData << (64 - countBits)) |
                        ((uint64_t)0x1 << (64 - countBits)); // дополнение, чтобы блок был равен 64 битам
        }
        if(countBlock * 8 > 10 * 1024 && countBlock * 8 <= 20 * 1024) // предупреждение если шифруем уже больше 10Кбайт
        {
            cout << "Зашифровано более 10 Кб!!!" << endl;
        } else if(countBlock * 8 >= 20 * 1024) {
            cout << "Зашифровано более 20 Кб!!! Завершаю работу"
                 << endl; // заканчиваем шифрование если шифруем уже больше 20Кбайт
            return 1;
        }
        blockData = mag.EncryptDecrypt_block(blockData, true);
        resultFile.write((char*)&blockData, sizeBlockInbytes);
    }
    forCryptFile.close();
    resultFile.close();
    cout << "Файл успешно зашифрован" << endl;
    return 0;
}
int decrypt_file()
{
    uint64_t key = read_key("key");

    ifstream forDeCryptFile("Result_crypt.enc", ios::binary);
    int countBlock = 0;
    const int sizeBlockInbytes = 8;
    ofstream resultFile("For_Decrypt_file.txt", ios::binary);
    Magma mag;
    mag.Deploy_key_into_4(key);
    mag.Divide_deployed_key_into_8();
    uint64_t blockData;
    while(true) {
        countBlock++;
        forDeCryptFile.read((char*)&blockData, sizeBlockInbytes);
        if(forDeCryptFile.eof()) {
            break;
        }
        blockData = mag.EncryptDecrypt_block(blockData, true);
        resultFile.write((char*)&blockData, sizeBlockInbytes);
    }
    forDeCryptFile.close();
    resultFile.close();
    cout << "Файл успешно расшифрован" << endl;
    return 0;
}
int main(int argc, char** argv)
{
    //    gen_key("key");
    encrypt_file();
    //    decrypt_file();
    return 0;
}
