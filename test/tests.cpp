// Copyright 2026 AnastasiaLos

#include <gtest/gtest.h>
#include <vector>
#include <fstream>
#include <string>
#include "textgen.h"

class MarkovTest : public ::testing::Test {
 protected:
    MarkovTextGenerator gen{2, 100};
};

TEST_F(MarkovTest, TestInitialState) {
    EXPECT_EQ(gen.getStatetab().size(), 0);
}

TEST_F(MarkovTest, TestAddSingleEntry) {
    MarkovTextGenerator::Prefix key;
    key.push_back("hello");
    key.push_back("world");
    gen.addEntry(key, "suffix");

    EXPECT_EQ(gen.getStatetab().size(), 1);
}

TEST_F(MarkovTest, TestGetRandomSuffixFromVector) {
    std::vector<std::string> suffixes = {"one", "two", "three"};
    std::string result = gen.getRandomSuffix(suffixes);
    EXPECT_TRUE(result == "one" || result == "two" || result == "three");
}

TEST_F(MarkovTest, TestGetRandomSuffixFromEmptyVector) {
    std::vector<std::string> suffixes;
    std::string result = gen.getRandomSuffix(suffixes);
    EXPECT_EQ(result, "");
}

TEST_F(MarkovTest, TestPrefixFromWords) {
    std::vector<std::string> words = {"a", "b", "c", "d", "e"};
    auto prefix = gen.getPrefixFromWords(words, 1);
    EXPECT_EQ(prefix.size(), 2);
    EXPECT_EQ(prefix[0], "b");
    EXPECT_EQ(prefix[1], "c");
}

TEST_F(MarkovTest, TestReadTextCreatesEntries) {
    std::ofstream testFile("test_input.txt");
    testFile << "one two three four five six seven";
    testFile.close();

    gen.readText("test_input.txt");
    EXPECT_GT(gen.getStatetab().size(), 0);

    std::remove("test_input.txt");
}

TEST_F(MarkovTest, TestMultipleSuffixesForSamePrefix) {
    MarkovTextGenerator::Prefix key;
    key.push_back("a");
    key.push_back("b");

    gen.addEntry(key, "x");
    gen.addEntry(key, "y");
    gen.addEntry(key, "z");

    auto it = gen.getStatetab().find(key);
    EXPECT_NE(it, gen.getStatetab().end());
    EXPECT_EQ(it->second.size(), 3);
}

TEST_F(MarkovTest, TestClearWorks) {
    MarkovTextGenerator::Prefix key;
    key.push_back("a");
    key.push_back("b");
    gen.addEntry(key, "x");
    EXPECT_GT(gen.getStatetab().size(), 0);

    gen.clear();
    EXPECT_EQ(gen.getStatetab().size(), 0);
}

TEST_F(MarkovTest, TestFileNotFound) {
    testing::internal::CaptureStderr();
    gen.readText("nonexistent_file.txt");
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_FALSE(output.empty());
}

TEST_F(MarkovTest, TestGenerateCreatesFile) {
    std::ofstream testFile("test_input.txt");
    testFile << "a b c a b c a b c";
    testFile.close();

    gen.readText("test_input.txt");
    gen.generate("test_output.txt");

    std::ifstream outputFile("test_output.txt");
    EXPECT_TRUE(outputFile.good());

    outputFile.close();
    std::remove("test_input.txt");
    std::remove("test_output.txt");
}

TEST_F(MarkovTest, TestDifferentPrefixSizes) {
    MarkovTextGenerator gen2(3, 100);
    EXPECT_EQ(gen2.getStatetab().size(), 0);

    std::ofstream testFile("test_input.txt");
    testFile << "a b c d e f g";
    testFile.close();

    gen2.readText("test_input.txt");
    EXPECT_GT(gen2.getStatetab().size(), 0);

    std::remove("test_input.txt");
}
