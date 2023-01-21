#include "DirectoryScanner.h"
#include "Resources.h"

#include "gmock/gmock.h"

#include <vector>

using namespace Homework;
using namespace std::string_literals;
namespace fs = boost::filesystem;

const std::string ROOT_DIR = BINARY_DIR + "/test-data/directory-scanner-test/"s;
const std::string INCLUDE_DIRS_TEST_DIR = ROOT_DIR + "include-dirs-test/"s;
const std::string EXCLUDE_DIRS_TEST_DIR = ROOT_DIR + "exclude-dirs-test/"s;
const std::string FILE_MASK_TEST_DIR = ROOT_DIR + "file-mask-test/"s;
const std::string MIN_FILE_SIZE_TEST_DIR = ROOT_DIR + "min-file-size-test/"s;
const std::string SCAN_LEVEL_TEST_DIR = ROOT_DIR + "scan-level-test/"s;

const std::size_t FILE_SIZE_BYTES = 5;
const std::size_t DEEP_SCAN_LEVEL = 100;

class StringMatcherMock : public StringMatcher {
public:
    StringMatcherMock() : StringMatcher(std::vector<std::string>()) {
    }

    MOCK_METHOD(bool, matches, (const std::string& str), (const override));
};

/**
 * A fake hash function. It is used only to set up DirectoryScanner. The result of the function doesn't matter.
 */
Hash getHash(const std::vector<char>&) {
    Hash hash;
    return hash;
}

DirectoryScanner createDirectoryScanner() {
    DirectoryScanner scanner;
    scanner.setFilenameMatcher(std::make_shared<StringMatcher>(std::vector<std::string>()));
    scanner.setHasher(getHash);
    scanner.setScanLevel(DEEP_SCAN_LEVEL);
    scanner.setMinFileSize(0);
    return scanner;
}

void verifyFiles(const std::vector<std::string>& expectedFiles, std::vector<std::unique_ptr<File>>& files) {
    ASSERT_EQ(expectedFiles.size(), files.size());

    for (std::size_t i = 0; i < expectedFiles.size(); ++i) {
        auto& expectedPath = expectedFiles[i];
        auto& file = files[i];
        ASSERT_TRUE(fs::equivalent(expectedPath, file->getFilepath()));
        ASSERT_EQ(fs::file_size(expectedPath), file->getFileSize());
    }
}

TEST(DirectoryScannerTest, getFilesFromDirectoriesIncludeDirs) {
    //given
    DirectoryScanner scanner = createDirectoryScanner();

    std::vector<std::string> dirsToScan = {
        INCLUDE_DIRS_TEST_DIR + "level-1-dir-1"s,
        INCLUDE_DIRS_TEST_DIR + "level-1-dir-2/level-2-dir-2"s
    };

    scanner.setDirectoriesToScan(dirsToScan);

    std::vector<std::string> expectedFiles = {
        INCLUDE_DIRS_TEST_DIR + "level-1-dir-1/f1.txt"s,
        INCLUDE_DIRS_TEST_DIR + "level-1-dir-2/level-2-dir-2/f1.txt"s,
    };

    //when
    auto files = scanner.getFilesFromDirectories();

    //then
    verifyFiles(expectedFiles, files);
}

TEST(DirectoryScannerTest, getFilesFromDirectoriesExcludeDirs) {
    //given
    DirectoryScanner scanner = createDirectoryScanner();

    std::vector<std::string> dirsToScan = { EXCLUDE_DIRS_TEST_DIR };
    std::vector<std::string> dirsToExclude = { 
        EXCLUDE_DIRS_TEST_DIR + "level-1-dir-1"s,
        EXCLUDE_DIRS_TEST_DIR + "level-1-dir-2/level-2-dir-2"s
    };

    scanner.setDirectoriesToScan(dirsToScan);
    scanner.setDirectoriesToExclude(dirsToExclude);

    std::vector<std::string> expectedFiles = {
        EXCLUDE_DIRS_TEST_DIR + "f1.txt"s,
        EXCLUDE_DIRS_TEST_DIR + "level-1-dir-2/f1.txt"s,
        EXCLUDE_DIRS_TEST_DIR + "level-1-dir-2/level-2-dir-1/f1.txt"s
    };

    //when
    auto files = scanner.getFilesFromDirectories();

    //then
    verifyFiles(expectedFiles, files);
}

TEST(DirectoryScannerTest, getFilesFromDirectoriesFileMask) {
    //given
    using ::testing::Return;

    auto filenameMatcher = std::make_shared<StringMatcherMock>();
    ON_CALL(*filenameMatcher, matches("f1.txt")).WillByDefault(Return(true));
    ON_CALL(*filenameMatcher, matches("f3.txt")).WillByDefault(Return(true));

    DirectoryScanner scanner;
    scanner.setFilenameMatcher(static_cast<std::shared_ptr<StringMatcher>>(filenameMatcher));
    scanner.setHasher(getHash);

    std::vector<std::string> dirsToScan = { FILE_MASK_TEST_DIR };
    scanner.setDirectoriesToScan(dirsToScan);

    std::vector<std::string> expectedFiles = {
        FILE_MASK_TEST_DIR + "f1.txt"s,
        FILE_MASK_TEST_DIR + "f3.txt"s
    };

    //when
    auto files = scanner.getFilesFromDirectories();

    //then
    verifyFiles(expectedFiles, files);
}

TEST(DirectoryScannerTest, getFilesFromDirectoriesScanLevel) {
    //given
    DirectoryScanner scanner = createDirectoryScanner();

    std::vector<std::string> dirsToScan = { SCAN_LEVEL_TEST_DIR };

    scanner.setDirectoriesToScan(dirsToScan);
    scanner.setScanLevel(2);

    std::vector<std::string> expectedFiles = {
        SCAN_LEVEL_TEST_DIR + "f1.txt"s,
        SCAN_LEVEL_TEST_DIR + "level-1/f1.txt"s,
        SCAN_LEVEL_TEST_DIR + "level-1/level-2/f1.txt"s,
    };

    //when
    auto files = scanner.getFilesFromDirectories();

    //then
    verifyFiles(expectedFiles, files);
}

TEST(DirectoryScannerTest, getFilesFromDirectoriesMinFileSize) {
    //given
    DirectoryScanner scanner = createDirectoryScanner();

    std::vector<std::string> dirsToScan = { MIN_FILE_SIZE_TEST_DIR };
    scanner.setDirectoriesToScan(dirsToScan);
    scanner.setMinFileSize(FILE_SIZE_BYTES);

    std::vector<std::string> expectedFiles = {
        MIN_FILE_SIZE_TEST_DIR + "big-file-1.txt"s,
        MIN_FILE_SIZE_TEST_DIR + "big-file-2.txt"s
    };

    //when
    auto files = scanner.getFilesFromDirectories();

    //then
    verifyFiles(expectedFiles, files);
}
