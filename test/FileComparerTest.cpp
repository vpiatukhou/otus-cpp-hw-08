#include "FileComparer.h"

#include "gtest/gtest.h"

using namespace Homework;

class TestFile : public File {
private:
    std::string filepath;
    std::vector<std::uint32_t> hashedBlocks;
    std::vector<std::uint32_t>::iterator it = hashedBlocks.begin();
    Hash currentBlockHash;

public:
    TestFile(const std::string& filepath_, std::vector<std::uint32_t>& hashedBlocks_) 
        : filepath(filepath_), hashedBlocks(hashedBlocks_) {
        currentBlockHash.resize(1);
    }

    bool readNextBlock() override {
        if (it != hashedBlocks.end()) {
            currentBlockHash[0] = *it;
            ++it;
        }
        return it != hashedBlocks.end();
    }

    std::string const& getFilepath() const override {
        return filepath;
    }

    FileSize getFileSize() const override {
        return hashedBlocks.size();
    }

    Hash const& getBlockHash() const override {
        return currentBlockHash;
    }
};

TEST(FileComparerTest, findDuplicateFilesEmpty) {
    //given
    std::vector<std::unique_ptr<File>> files;

    //when
    auto result = findDuplicateFiles(files);

    //then
    ASSERT_TRUE(result.empty());
}

TEST(FileComparerTest, findDuplicateFilesSimplestCasePositive) {
    //given
    std::vector<std::uint32_t> hashes = { 1 };
    std::vector<std::unique_ptr<File>> files;
    files.push_back(std::make_unique<TestFile>("f1", hashes));
    files.push_back(std::make_unique<TestFile>("f2", hashes));

    std::list<std::list<std::string>> expected = { {"f1", "f2"} };

    //when
    auto result = findDuplicateFiles(files);

    //then
    ASSERT_EQ(expected, result);
}

TEST(FileComparerTest, findDuplicateFilesDifferentSize) {
    //given
    std::vector<std::uint32_t> hashes1 = { 1 };
    std::vector<std::uint32_t> hashes2 = { 1, 2 };
    std::vector<std::unique_ptr<File>> files;
    files.push_back(std::make_unique<TestFile>("f1", hashes1));
    files.push_back(std::make_unique<TestFile>("f2", hashes2));

    //when
    auto result = findDuplicateFiles(files);

    //then
    ASSERT_TRUE(result.empty());
}

TEST(FileComparerTest, findDuplicateFilesDifferentContent) {
    //given
    std::vector<std::uint32_t> hashes1 = { 1 };
    std::vector<std::uint32_t> hashes2 = { 2 };
    std::vector<std::unique_ptr<File>> files;
    files.push_back(std::make_unique<TestFile>("f1", hashes1));
    files.push_back(std::make_unique<TestFile>("f2", hashes2));

    //when
    auto result = findDuplicateFiles(files);

    //then
    ASSERT_TRUE(result.empty());
}

TEST(FileComparerTest, findDuplicateFilesDifferentBlockInMiddle) {
    //given
    std::vector<std::unique_ptr<File>> files;
    //all blocks are the same except of the one in the middle
    std::vector<std::uint32_t> hashes1 = { 1, 2, 3, 4, 5 };
    std::vector<std::uint32_t> hashes2 = { 1, 2, 9, 4, 5 };
    files.push_back(std::make_unique<TestFile>("f1", hashes1));
    files.push_back(std::make_unique<TestFile>("f2", hashes1));
    files.push_back(std::make_unique<TestFile>("f3", hashes2));
    files.push_back(std::make_unique<TestFile>("f4", hashes2));

    std::list<std::list<std::string>> expected = { {"f1", "f2"}, {"f3", "f4"} };

    //when
    auto result = findDuplicateFiles(files);

    //then
    ASSERT_EQ(expected, result);
}

TEST(FileComparerTest, findDuplicateFilesComplex) {
    //given
    std::vector<std::unique_ptr<File>> files;
    //a file with unique size
    std::vector<std::uint32_t> hashes1 = { 1 };
    files.push_back(std::make_unique<TestFile>("f1", hashes1));
    //files with same size and content
    std::vector<std::uint32_t> hashes2 = { 1, 2 };
    files.push_back(std::make_unique<TestFile>("f2", hashes2));
    files.push_back(std::make_unique<TestFile>("f3", hashes2));
    files.push_back(std::make_unique<TestFile>("f4", hashes2));
    //files with same size and different content
    std::vector<std::uint32_t> hashes3 = { 1, 2, 3, 4, 5 };
    std::vector<std::uint32_t> hashes4 = { 1, 2, 9, 4, 5 };
    //std::vector<std::uint32_t> hashes5 = { 1, 2, 3, 4, 5 };
    std::vector<std::uint32_t> hashes6 = { 1, 2, 3, 4, 9 };
    files.push_back(std::make_unique<TestFile>("f5", hashes3));
    files.push_back(std::make_unique<TestFile>("f6", hashes3));
    files.push_back(std::make_unique<TestFile>("f7", hashes4));
    files.push_back(std::make_unique<TestFile>("f8", hashes3)); //test sorting
    files.push_back(std::make_unique<TestFile>("f9", hashes4));
    files.push_back(std::make_unique<TestFile>("f10", hashes6));
    files.push_back(std::make_unique<TestFile>("f11", hashes6));

    std::list<std::list<std::string>> expected = { {"f2", "f3", "f4"}, {"f5", "f6", "f8"}, {"f7", "f9"}, {"f10", "f11"} };

    //when
    auto result = findDuplicateFiles(files);

    //then
    ASSERT_EQ(expected, result);
}
