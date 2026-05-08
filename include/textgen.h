#ifndef TEXTGEN_H
#define TEXTGEN_H

#include <deque>
#include <map>
#include <vector>
#include <string>
#include <random>

class MarkovTextGenerator {
public:
    using Prefix = std::deque<std::string>;
    using StateTable = std::map<Prefix, std::vector<std::string>>;

private:
    StateTable statetab;
    std::mt19937 rng;
    int npref;
    int maxgen;

public:
    MarkovTextGenerator(int npref = 2, int maxgen = 1000);
    
    void readText(const std::string& filename);
    void generate(const std::string& outputFilename);
    
    // Для тестов
    StateTable getStatetab() const { return statetab; }
    void addEntry(const Prefix& key, const std::string& suffix);
    std::string getRandomSuffix(const std::vector<std::string>& suffixes);
    Prefix getPrefixFromWords(const std::vector<std::string>& words, int start);
    void clear();
};

#endif
