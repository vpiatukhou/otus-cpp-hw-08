#include "DirectoryScanner.h"
#include "FileComparer.h"
#include "HasherFactory.h"
#include "Util.h"

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>

using namespace Homework;

namespace po = boost::program_options;

const std::string HELP_PARAM = "help";

const std::string UNEXPECTED_ERROR_MSG = "Unexpected error occurred: ";
const int UNEXPECTED_ERROR_CODE = -1;
const int INVALID_INPUT_ERROR_CODE = -2;

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

int main(int argc, char** argv) {
    std::vector<std::string> dirsToScan, dirsToExclude, fileMasks;
    size_t scanLevel;
    FileSize blockSize;
    std::string minFileSize, hashAlgorithm;

    auto dirsToScanOption = po::value<std::vector<std::string>>(&dirsToScan)->multitoken();
    auto dirsToExcludeOption = po::value<std::vector<std::string>>(&dirsToExclude)->multitoken();
    auto scanLevelOption = po::value<std::size_t>(&scanLevel)->default_value(DirectoryScanner::DEFAULT_SCAN_LEVEL);
    auto minFileSizeOption = po::value<std::string>(&minFileSize)->default_value(std::to_string(DirectoryScanner::DEFAULT_MIN_FILE_SIZE_BYTE));
    auto fileMaskOption = po::value<std::vector<std::string>>(&fileMasks)->multitoken();
    auto blockSizeOption = po::value<FileSize>(&blockSize)->default_value(DirectoryScanner::DEFAULT_BLOCK_SIZE_BYTE);
    auto hashAlgorithmOption = po::value<std::string>(&hashAlgorithm)->default_value(CRC32);

    po::options_description optionsDescription("Options");
    optionsDescription.add_options()
        ("help,h", "Print help information.")
        ("include-path,i", dirsToScanOption, "Paths to be scanned. If not specified, the scan starts in the current directory.")
        ("exclude-path,e", dirsToExcludeOption, "Paths which must not be scanned.")
        ("level,l", scanLevelOption, "A scan level. 0 - only the current directory will be scanned. 1 - the current directory and its subdirectories and so on.")
        ("min-file-size,s", minFileSizeOption, "A minimum file size. Files which are smaller than the specified size will be skipped.\n"
            "Examples:\n"
            "1 - one byte\n"
            "1k - one kilobyte\n"
            "1m - one megabyte\n"
            "1g - one gigabyte\n")
        ("file-name,n", fileMaskOption, "File name masks. A file is scanned only if its name matches to one of the specified masks.")
        ("block-size,S", blockSizeOption, "A number of bytes which are read per one I/O operation.")
        ("hash-algorithm,H", hashAlgorithmOption, "A hash algorithm. Supported values are: crc32 and md5.");
    
    po::variables_map optionValues;
    po::store(po::parse_command_line(argc, argv, optionsDescription), optionValues);
    po::notify(optionValues);

    if (optionValues.count(HELP_PARAM)) {
        std::cout << optionsDescription << std::endl;
    } else {
        DirectoryScanner scanner;
        scanner.setDirectoriesToScan(dirsToScan);
        scanner.setDirectoriesToExclude(dirsToExclude);
        scanner.setFileMasks(fileMasks);
        scanner.setScanLevel(scanLevel);

        try {
            scanner.setMinFileSize(fileSizeToBytes(minFileSize));
        } catch (std::invalid_argument& e) {
            std::cout << "An invalid value was specified for min-file-size: " << e.what() << std::endl;
            return INVALID_INPUT_ERROR_CODE;
        }
        scanner.setBlockSize(blockSize);

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
