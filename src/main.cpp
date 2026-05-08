// Copyright 2026 AnastasiaLos

#include <iostream>
#include <string>
#include "textgen.h"

int main() {
    const int NPREF = 2;
    const int MAXGEN = 1200;

    MarkovTextGenerator generator(NPREF, MAXGEN);

    std::cout << "========================================" << std::endl;
    std::cout << "Генератор текста на основе цепи Маркова" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Размер префикса: " << NPREF << " слова" << std::endl;
    std::cout << "Длина текста: " << MAXGEN << " слов" << std::endl;
    std::cout << std::endl;

    // Пробуем открыть файл с текстом
    std::string inputFile = "input.txt";
    std::cout << "Введите имя входного файла (Enter для input.txt): ";
    std::getline(std::cin, inputFile);
    if (inputFile.empty()) {
        inputFile = "input.txt";
    }

    generator.readText(inputFile);

    if (generator.getStatetab().empty()) {
        std::cerr << "Не удалось прочитать файл или файл пуст!" << std::endl;
        return 1;
    }

    std::cout << "Таблица построена. Количество префиксов: "
              << generator.getStatetab().size() << std::endl;

    // Генерируем текст
    std::string outputFile = "result/gen.txt";
    generator.generate(outputFile);

    std::cout << std::endl;
    std::cout << "Готово!" << std::endl;

    return 0;
}
