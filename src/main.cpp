#include "DirectoryScanner.h"
#include "FileComparer.h"
#include "HasherFactory.h"
#include "ProgramOptions.h"

#include <iostream>

const std::string UNEXPECTED_ERROR_MSG = "Unexpected error occurred: ";

const int INVALID_INPUT_ERROR_CODE = -2;
const int UNEXPECTED_ERROR_CODE = -1;

void sortFilepaths(std::list<std::list<std::string>>& groupedFilepaths) {
    for (auto& duplicateFiles : groupedFilepaths) {
        duplicateFiles.sort();
    }
    groupedFilepaths.sort();
}

void printFilepaths(const std::list<std::list<std::string>>& groupedFilepaths) {
    for (auto& duplicateFiles : groupedFilepaths) {
        for (auto& filepath : duplicateFiles) {
            std::cout << filepath << std::endl;
        }
        std::cout << std::endl;
    }
}

int main(int argc, char* argv[]) {
    using namespace Homework;

    ProgramOptions programOptions;
    if (programOptions.parse(argc, argv, std::cout)) {
        DirectoryScanner scanner;

        try {
            scanner.setDirectoriesToScan(programOptions.getDirectoriesToScan());
            scanner.setDirectoriesToExclude(programOptions.getDirectoriesToExclude());
            scanner.setFilenameMatcher(std::make_shared<StringMatcher>(programOptions.getFileMasks()));
            scanner.setScanLevel(programOptions.getScanLevel());
            scanner.setMinFileSize(programOptions.getMinFileSize());
            scanner.setBlockSize(programOptions.getBlockSize());
            auto hasher = getHasher(programOptions.getHashAlgorithm());
            scanner.setHasher(hasher);

            auto files = scanner.getFilesFromDirectories();
            auto groupedFilepaths = findDuplicateFiles(files);
            sortFilepaths(groupedFilepaths);
            printFilepaths(groupedFilepaths);
        } catch (std::invalid_argument& e) {
            std::cerr << e.what() << std::endl;
            return INVALID_INPUT_ERROR_CODE;
        } catch (std::exception& e) {
            std::cerr << UNEXPECTED_ERROR_MSG << e.what() << std::endl;
            return UNEXPECTED_ERROR_CODE;
        }
    }
    return 0;
}
