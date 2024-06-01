#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <algorithm>
#include "gost_magma.h"

const size_t BLOCK_SIZE = 8; // 64 бита = 8 байт
const size_t KEY_SIZE = 7; // 56 бит = 7 байт
const size_t EXTENDED_KEY_SIZE = 32; // 256 бит = 32 байта

// Функция для чтения ключа из файла
std::vector<uint8_t> readKey(const std::string &filename) {
    std::ifstream keyFile(filename, std::ios::binary);
    if (!keyFile) {
        throw std::runtime_error("Не удалось открыть файл ключа.");
    }
    std::vector<uint8_t> key(KEY_SIZE);
    keyFile.read(reinterpret_cast<char*>(key.data()), KEY_SIZE);
    if (keyFile.gcount() != KEY_SIZE) {
        throw std::runtime_error("Некорректный размер ключа в файле.");
    }
    return key;
}

// Функция для расширения ключа до 256 бит
std::vector<uint8_t> extendKey(const std::vector<uint8_t> &key) {
    std::vector<uint8_t> extendedKey(EXTENDED_KEY_SIZE);
    for (size_t i = 0; i < EXTENDED_KEY_SIZE; ++i) {
        extendedKey[i] = key[i % KEY_SIZE];
    }
    return extendedKey;
}

// Функция дополнения данных по процедуре 2
std::vector<uint8_t> padData(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> paddedData = data;
    size_t padLen = BLOCK_SIZE - (data.size() % BLOCK_SIZE);
    paddedData.push_back(0x80); // Добавляем 1 бит
    for (size_t i = 1; i < padLen; ++i) {
        paddedData.push_back(0x00); // Добавляем 0 биты
    }
    return paddedData;
}

// Функция шифрования файла
void encryptFile(const std::string &inputFilename, const std::string &outputFilename, const std::vector<uint8_t> &key) {
    std::ifstream inputFile(inputFilename, std::ios::binary);
    std::ofstream outputFile(outputFilename, std::ios::binary);
    if (!inputFile || !outputFile) {
        throw std::runtime_error("Не удалось открыть входной или выходной файл.");
    }

    std::vector<uint8_t> buffer(BLOCK_SIZE);
    size_t totalBytesProcessed = 0;

    while (inputFile.read(reinterpret_cast<char*>(buffer.data()), BLOCK_SIZE) || inputFile.gcount() > 0) {
        size_t bytesRead = inputFile.gcount();
        if (bytesRead < BLOCK_SIZE) {
            buffer.resize(bytesRead);
            buffer = padData(buffer);
        }
        std::vector<uint8_t> encryptedBlock(BLOCK_SIZE);
        encryptBlock(buffer.data(), encryptedBlock.data(), key);
        outputFile.write(reinterpret_cast<char*>(encryptedBlock.data()), BLOCK_SIZE);
        totalBytesProcessed += bytesRead;

        if (totalBytesProcessed > 10 * 1024) {
            std::cerr << "Предупреждение: Заканчивается срок действия ключа.\n";
        }
        if (totalBytesProcessed > 20 * 1024) {
            throw std::runtime_error("Превышен лимит зашифрования на одном ключе.");
        }
    }
}

// Функция расшифрования файла
void decryptFile(const std::string &inputFilename, const std::string &outputFilename, const std::vector<uint8_t> &key) {
    std::ifstream inputFile(inputFilename, std::ios::binary);
    std::ofstream outputFile(outputFilename, std::ios::binary);
    if (!inputFile || !outputFile) {
        throw std::runtime_error("Не удалось открыть входной или выходной файл.");
    }

    std::vector<uint8_t> buffer(BLOCK_SIZE);

    while (inputFile.read(reinterpret_cast<char*>(buffer.data()), BLOCK_SIZE) || inputFile.gcount() > 0) {
        std::vector<uint8_t> decryptedBlock(BLOCK_SIZE);
        decryptBlock(buffer.data(), decryptedBlock.data(), key);
        outputFile.write(reinterpret_cast<char*>(decryptedBlock.data()), BLOCK_SIZE);
    }
}

// Функция генерации ключа
void generateKey(const std::string &filename) {
    std::ofstream keyFile(filename, std::ios::binary);
    if (!keyFile) {
        throw std::runtime_error("Не удалось создать файл ключа.");
    }
    std::vector<uint8_t> key(KEY_SIZE);
    std::generate(key.begin(), key.end(), std::rand);
    keyFile.write(reinterpret_cast<const char*>(key.data()), KEY_SIZE);
}

// Функция для выполнения дополнительных тестов
void performTests(const std::string &encryptedFilename, const std::vector<uint8_t> &key) {
    std::ifstream inputFile(encryptedFilename, std::ios::binary);
    if (!inputFile) {
        throw std::runtime_error("Не удалось открыть файл шифртекста.");
    }

    // Считываем весь шифртекст
    std::vector<uint8_t> ciphertext((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();

    // Выполняем различные искажения
    std::vector<std::vector<uint8_t>> modifiedCiphertexts;

    // Удаление 1 байта
    if (ciphertext.size() > 1) {
        std::vector<uint8_t> modified(ciphertext);
        modified.erase(modified.begin() + 1);
        modifiedCiphertexts.push_back(modified);
    }

    // Удаление блока данных (некратно 64 битам)
    if (ciphertext.size() > BLOCK_SIZE + 3) {
        std::vector<uint8_t> modified(ciphertext);
        modified.erase(modified.begin() + BLOCK_SIZE - 3, modified.begin() + BLOCK_SIZE);
        modifiedCiphertexts.push_back(modified);
    }

    // Удаление блока данных (кратно 64 битам)
    if (ciphertext.size() > 2 * BLOCK_SIZE) {
        std::vector<uint8_t> modified(ciphertext);
        modified.erase(modified.begin() + BLOCK_SIZE, modified.begin() + 2 * BLOCK_SIZE);
        modifiedCiphertexts.push_back(modified);
    }

    // Добавление блока данных (кратно 64 битам)
    {
        std::vector<uint8_t> modified(ciphertext);
        std::vector<uint8_t> additionalBlock(BLOCK_SIZE, 0xAA); // Добавляем блок с произвольными данными
        modified.insert(modified.begin() + BLOCK_SIZE, additionalBlock.begin(), additionalBlock.end());
        modifiedCiphertexts.push_back(modified);
    }

    // Перестановка двух блоков данных (кратно 64 битам)
    if (ciphertext.size() > 2 * BLOCK_SIZE) {
        std::vector<uint8_t> modified(ciphertext);
        std::swap_ranges(modified.begin(), modified.begin() + BLOCK_SIZE, modified.begin() + BLOCK_SIZE);
        modifiedCiphertexts.push_back(modified);
    }

    // Пытаемся расшифровать модифицированные файлы
    for (size_t i = 0; i < modifiedCiphertexts.size(); ++i) {
        std::string outputFilename = "modified_decrypted_" + std::to_string(i) + ".txt";
        std::ofstream outputFile(outputFilename, std::ios::binary);
        if (!outputFile) {
            throw std::runtime_error("Не удалось создать файл для расшифрованного текста.");
        }
        std::vector<uint8_t> buffer(BLOCK_SIZE);
        inputFile.clear();
        inputFile.seekg(0);
        for (size_t j = 0; j < modifiedCiphertexts[i].size(); j += BLOCK_SIZE) {
            std::memcpy(buffer.data(), modifiedCiphertexts[i].data() + j, BLOCK_SIZE);
            std::vector<uint8_t> decryptedBlock(BLOCK_SIZE);
            decryptBlock(buffer.data(), decryptedBlock.data(), key);
            outputFile.write(reinterpret_cast<char*>(decryptedBlock.data()), BLOCK_SIZE);
        }
    }
}

// Главная функция
int main() {
    try {
        std::string keyFilename = "key.key";
        std::string inputFilename = "1.txt";
        std::string encryptedFilename = "ciphertext.enc";
        std::string decryptedFilename = "decrypted.txt";
        
        // Генерация ключа
        generateKey(keyFilename);

        // Считываем и расширяем ключ
        std::vector<uint8_t> key = readKey(keyFilename);
        std::vector<uint8_t> extendedKey = extendKey(key);

        // Шифруем файл
        encryptFile(inputFilename, encryptedFilename, extendedKey);

        // Расшифровываем файл
        decryptFile(encryptedFilename, decryptedFilename, extendedKey);

        std::cout << "Шифрование и расшифрование завершены успешно.\n";

        // Выполняем дополнительные тесты
        performTests(encryptedFilename, extendedKey);

        // Проверка блокировки при шифровании файла более 20 Кб
        std::string largeInputFilename = "large_plaintext.txt";
        std::string largeEncryptedFilename = "large_ciphertext.enc";
        try {
            encryptFile(largeInputFilename, largeEncryptedFilename, extendedKey);
        } catch (const std::exception &e) {
            std::cerr << "Ожидаемая ошибка: " << e.what() << '\n';
        }

    } catch (const std::exception &e) {
        std::cerr << "Ошибка: " << e.what() << '\n';
    }

    return 0;
}
		