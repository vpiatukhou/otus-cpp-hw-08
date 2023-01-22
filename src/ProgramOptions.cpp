#include "ConversionUtil.h"
#include "HasherFactory.h"
#include "ProgramOptions.h"

#include <stdexcept>

#include <boost/program_options.hpp>

namespace Homework {

const std::string HELP_PARAM = "help";
const std::size_t DEFAULT_SCAN_LEVEL = 0;
const FileSize DEFAULT_MIN_FILE_SIZE_BYTE = 2;
const std::size_t DEFAULT_BLOCK_SIZE_BYTE = 1024;

bool ProgramOptions::parse(int argc, char* argv[], std::ostream& out) {
    using namespace std::string_literals;
    using string = std::string;

    namespace po = boost::program_options;

    string scanLevelStr, minFileSizeStr, blockSizeStr;

    auto dirsToScanOption = po::value<std::vector<string>>(&directoriesToScan)->multitoken();
    auto dirsToExcludeOption = po::value<std::vector<string>>(&directoriesToExclude)->multitoken();
    auto scanLevelOption = po::value<string>(&scanLevelStr)->default_value(std::to_string(DEFAULT_SCAN_LEVEL));
    auto minFileSizeOption = po::value<string>(&minFileSizeStr)->default_value(std::to_string(DEFAULT_MIN_FILE_SIZE_BYTE));
    auto fileMaskOption = po::value<std::vector<string>>(&fileMasks)->multitoken();
    auto blockSizeOption = po::value<string>(&blockSizeStr)->default_value(std::to_string(DEFAULT_BLOCK_SIZE_BYTE));
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
        out << optionsDescription << std::endl;
    }
    try {
        scanLevel = stringToSize(scanLevelStr);
    } catch (std::invalid_argument& e) {
        throw std::invalid_argument("The scan level is invalid: "s + e.what());
    }

    try {
        minFileSize = fileSizeToBytes(minFileSizeStr);
    } catch (std::invalid_argument& e) {
        throw std::invalid_argument("The minimum file size is invalid: "s + e.what());
    }

    try {
        blockSize = fileSizeToBytes(blockSizeStr);
    } catch (std::invalid_argument& e) {
        throw std::invalid_argument("The block size is invalid: "s + e.what());
    }
    return true;
}

std::vector<std::string> ProgramOptions::getDirectoriesToScan() const {
    return directoriesToScan;
}

std::vector<std::string> ProgramOptions::getDirectoriesToExclude() const {
    return directoriesToExclude;
}

std::vector<std::string> ProgramOptions::getFileMasks() const {
    return fileMasks;
}

std::size_t ProgramOptions::getScanLevel() const {
    return scanLevel;
}

FileSize ProgramOptions::getMinFileSize() const {
    return minFileSize;
}

FileSize ProgramOptions::getBlockSize() const {
    return blockSize;
}

std::string ProgramOptions::getHashAlgorithm() const {
    return hashAlgorithm;
}

};
