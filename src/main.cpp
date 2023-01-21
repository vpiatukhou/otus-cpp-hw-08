#include "DirectoryScanner.h"
#include "FileComparer.h"
#include "HasherFactory.h"
#include "ConversionUtil.h"

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>

#include <boost/program_options.hpp>

using namespace Homework;
using string = std::string;
namespace po = boost::program_options;

const string HELP_PARAM = "help";

const string UNEXPECTED_ERROR_MSG = "Unexpected error occurred: ";
const int UNEXPECTED_ERROR_CODE = -1;
const int INVALID_INPUT_ERROR_CODE = -2;

void sortFilepaths(std::list<std::list<string>>& groupedFilepaths) {
    for (auto& duplicateFiles : groupedFilepaths) {
        duplicateFiles.sort();
    }
    groupedFilepaths.sort();
}

void printFilepaths(const std::list<std::list<string>>& groupedFilepaths) {
    for (auto& duplicateFiles : groupedFilepaths) {
        for (auto& filepath : duplicateFiles) {
            std::cout << filepath << std::endl;
        }
        std::cout << std::endl;
    }
}

int main(int argc, char** argv) {
    std::vector<string> dirsToScan, dirsToExclude, fileMasks;
    string scanLevel, minFileSize, blockSize, hashAlgorithm;

    auto dirsToScanOption = po::value<std::vector<string>>(&dirsToScan)->multitoken();
    auto dirsToExcludeOption = po::value<std::vector<string>>(&dirsToExclude)->multitoken();
    auto scanLevelOption = po::value<string>(&scanLevel)->default_value(std::to_string(DirectoryScanner::DEFAULT_SCAN_LEVEL));
    auto minFileSizeOption = po::value<string>(&minFileSize)->default_value(std::to_string(DirectoryScanner::DEFAULT_MIN_FILE_SIZE_BYTE));
    auto fileMaskOption = po::value<std::vector<string>>(&fileMasks)->multitoken();
    auto blockSizeOption = po::value<string>(&blockSize)->default_value(std::to_string(DirectoryScanner::DEFAULT_BLOCK_SIZE_BYTE));
    auto hashAlgorithmOption = po::value<string>(&hashAlgorithm)->default_value(CRC32);

    po::options_description optionsDescription("Options");
    optionsDescription.add_options()
        ("help,h", "Print help information.")
        ("include-path,i", dirsToScanOption, "One or more paths to be scanned. If not specified, the scan starts in the current directory.\n\nExample:\n-i /absolute/path .. /relative/path\n")
        ("exclude-path,e", dirsToExcludeOption, "One or more paths which must not be scanned.\n\nExample:\n\n-e /absolute/path ../relative/path\n")
        ("level,l", scanLevelOption, "A subdirectory level. E.g. 0 (the default value) means that only the current directory is scanned. 1 - the current directory and its subdirectories. And so on.\n")
        ("min-file-size,s", minFileSizeOption, "A minimum file size. Files which are smaller than the specified size will be skipped. The default value is 2.\n\nExamples:\n\n1 - one byte\n1k - one kibibyte\n1m - one megibyte\n1g - one gibibyte.\n")
        ("file-name,n", fileMaskOption, "File name pattern.\n\nSpecial characters:\n\n*    Any zero or more characters.\n?    Any one character.\n\\    Removes special meaning of '?'.E.g.: \\ ? means 'a question mark' (not a wildcard).\n")
        ("block-size,b", blockSizeOption, "A number of bytes which are read per one I/O operation. The default value is 1024.\n\nExamples:\n\n1 - one byte\n1k - one kibibyte\n1m - one megibyte\n1g - one gibibyte.\n")
        ("hash-algorithm,H", hashAlgorithmOption, "A hash algorithm. Supported values are: crc32 (default) and md5.\n");
    
    po::variables_map optionValues;
    po::store(po::parse_command_line(argc, argv, optionsDescription), optionValues);
    po::notify(optionValues);

    if (optionValues.count(HELP_PARAM)) {
        std::cout << optionsDescription << std::endl;
    } else {
        DirectoryScanner scanner;
        scanner.setDirectoriesToScan(dirsToScan);
        scanner.setDirectoriesToExclude(dirsToExclude);
        scanner.setFilenameMatcher(std::make_shared<StringMatcher>(fileMasks));
        
        try {
            scanner.setScanLevel(stringToSize(scanLevel));
        } catch (std::invalid_argument& e) {
            std::cout << "The scan level is invalid: " << e.what() << std::endl;
            return INVALID_INPUT_ERROR_CODE;
        }

        try {
            scanner.setMinFileSize(fileSizeToBytes(minFileSize));
        } catch (std::invalid_argument& e) {
            std::cout << "The minimum file size is invalid: " << e.what() << std::endl;
            return INVALID_INPUT_ERROR_CODE;
        }

        try {
            scanner.setBlockSize(fileSizeToBytes(blockSize));
        } catch (std::invalid_argument& e) {
            std::cout << "The block size is invalid: " << e.what() << std::endl;
            return INVALID_INPUT_ERROR_CODE;
        }

        try {
            auto hasher = getHasher(hashAlgorithm);
            scanner.setHasher(hasher);
        } catch (std::invalid_argument& e) {
            std::cerr << e.what() << std::endl;
            return INVALID_INPUT_ERROR_CODE;
        }

        try {
            auto files = scanner.getFilesFromDirectories();
            auto groupedFilepaths = findDuplicateFiles(files);
            sortFilepaths(groupedFilepaths);
            printFilepaths(groupedFilepaths);
        } catch (std::exception& e) {
            std::cerr << UNEXPECTED_ERROR_MSG << e.what() << std::endl;
            return UNEXPECTED_ERROR_CODE;
        }
    }
    return 0;
}
