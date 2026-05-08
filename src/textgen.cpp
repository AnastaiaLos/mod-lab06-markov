// Copyright 2026 AnastasiaLos

#include "textgen.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <vector>
#include <string>

MarkovTextGenerator::MarkovTextGenerator(int npref, int maxgen)
    : npref(npref), maxgen(maxgen), rng(std::time(nullptr)) {}

void MarkovTextGenerator::clear() {
    statetab.clear();
}

void MarkovTextGenerator::readText(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл: " << filename << std::endl;
        return;
    }

    clear();

    std::string word;
    std::vector<std::string> words;

    while (file >> word) {
        // Удаляем пунктуацию
        word.erase(std::remove_if(word.begin(), word.end(),
            [](char c) { return ispunct(c) && c != '-'; }), word.end());
        // Приводим к нижнему регистру
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        if (!word.empty()) {
            words.push_back(word);
        }
    }

    if (words.size() < static_cast<size_t>(npref + 1)) {
        std::cerr << "Файл слишком короткий" << std::endl;
        return;
    }

    for (size_t i = 0; i <= words.size() - npref - 1; i++) {
        Prefix key;
        for (int j = 0; j < npref; j++) {
            key.push_back(words[i + j]);
        }
        std::string suffix = words[i + npref];
        statetab[key].push_back(suffix);
    }

    file.close();
}

std::string MarkovTextGenerator::getRandomSuffix(
    const std::vector<std::string>& suffixes) {
    if (suffixes.empty()) return "";
    std::uniform_int_distribution<int> dist(0, suffixes.size() - 1);
    return suffixes[dist(rng)];
}

void MarkovTextGenerator::addEntry(const Prefix& key,
                                   const std::string& suffix) {
    statetab[key].push_back(suffix);
}

MarkovTextGenerator::Prefix MarkovTextGenerator::getPrefixFromWords(
    const std::vector<std::string>& words, int start) {
    Prefix result;
    for (int i = 0; i < npref; i++) {
        if (start + i < static_cast<int>(words.size())) {
            result.push_back(words[start + i]);
        }
    }
    return result;
}

void MarkovTextGenerator::generate(const std::string& outputFilename) {
    std::ofstream output(outputFilename);
    if (!output.is_open()) {
        std::cerr << "Не удалось создать выходной файл: "
                  << outputFilename << std::endl;
        return;
    }

    if (statetab.empty()) {
        std::cerr << "Таблица пуста. Сначала прочитайте текст." << std::endl;
        return;
    }

    Prefix current = statetab.begin()->first;

    // Выводим начальный префикс
    for (const auto& word : current) {
        output << word << " ";
    }

    int generated = npref;

    while (generated < maxgen) {
        auto it = statetab.find(current);
        if (it == statetab.end() || it->second.empty()) {
            break;
        }

        std::string nextWord = getRandomSuffix(it->second);
        output << nextWord << " ";

        current.pop_front();
        current.push_back(nextWord);
        generated++;
    }

    output.close();
    std::cout << "Текст сгенерирован в файл: " << outputFilename << std::endl;
    std::cout << "Всего слов: " << generated << std::endl;
}
