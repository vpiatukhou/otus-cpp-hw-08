#include "FileComparer.h"

#include <boost/bimap.hpp>
#include <boost/bimap/multiset_of.hpp>
#include <boost/bimap/unordered_set_of.hpp>

#include <cassert>
#include <deque>
#include <map>
#include <unordered_map>

namespace Homework {

auto FILE_BLOCK_COMPARATOR = [](const std::unique_ptr<File>& a, const std::unique_ptr<File>& b) {
    return *a < *b;
};

FileComparer::FileComparer(FileSize blockSize_, const Hasher& hasher_) : blockSize(blockSize_), hasher(hasher_) {
}

/**
 * Filds files with the same content and adds their paths to the target list.
 * 
 * @param files - the list of files which will be compared. Must contain at least two files. All files must be of the same size.
 * @param target - the list of filepaths. An each sublist contains paths of the files with the same content.
 */
void FileComparer::findAndAppendDuplicates(GroupOfFiles& files, std::list<GroupOfPaths>& target) const {
    //the list contains groups of files. An each group contains files with the same content
    std::list<GroupOfFiles> processedGroups;

    //read first block of all files
    if (!readNextBlock(files)) {
        //all files are empty, thus all of them are duplicate
        processedGroups.push_back(std::move(files));
        filesToPaths(processedGroups, target);
        return;
    }

    //the list contains groups of files which are not totally processed yet.
    //An each group contains files which are considered equal at the moment.
    std::deque<GroupOfFiles> unprocessedGroups;
    unprocessedGroups.push_back(std::move(files)); //all files belong to the same group on the first iteration

    while (!unprocessedGroups.empty()) {
        GroupOfFiles& currentGroup = unprocessedGroups.front();
        if (currentGroup.size() == 1) {
            unprocessedGroups.erase(unprocessedGroups.begin());
            break;
        }

        bool hasNextBlock = true;

        //Read files block by block in the loop. Compare hashes of the blocks. 
        //The files with unique hashes are removed from the list.
        //Break the loop if no block are left or or all files have been removed from the list.
        while (hasNextBlock && !currentGroup.empty()) {
            currentGroup.sort(FILE_BLOCK_COMPARATOR);

            auto current = currentGroup.begin();
            auto next = currentGroup.begin();
            ++next; //it is guaranteed that there are at least two elements in the list

            std::size_t numberOfDuplicates = 0;

            bool mustRemoveLastFile = false;
            
            //compare the current blocks of all files in the list. Files with unique blocks will be removed from the list
            while (next != currentGroup.end()) {
                mustRemoveLastFile = false;
                if (**current == **next) {
                    ++next;
                    ++current;
                    ++numberOfDuplicates;
                } else {
                    if (numberOfDuplicates == 0) { //the current file is unique and will be removed from the list
                        ++next;
                        current = currentGroup.erase(current);
                        mustRemoveLastFile = true;
                    } else { //the current file is not unique but it differs from the next file
                        //move remaining files to the new group and break the loop
                        unprocessedGroups.emplace_back();
                        unprocessedGroups.back().splice(unprocessedGroups.back().end(), currentGroup, next, currentGroup.end());
                        numberOfDuplicates = 0;
                        break;
                    }
                }
            }

            //if the last file in the list is unique, remove it
            if (mustRemoveLastFile) {
                currentGroup.erase(current);
            }

            //all files in the current group have been compared, so we can read the next block
            hasNextBlock = readNextBlock(currentGroup);
        }

        if (!unprocessedGroups.front().empty()) {
            processedGroups.push_back(std::move(unprocessedGroups.front()));
        }
        unprocessedGroups.erase(unprocessedGroups.begin());
    }

    filesToPaths(processedGroups, target);
}

bool FileComparer::readNextBlock(GroupOfFiles& files) const {
    bool hasNextBlock = false;
    for (auto& file : files) {
        //we use 'hasNextBlock =' instead of 'hasNextBlock &=' because all files have the same size
        hasNextBlock = file->readNextBlock(blockSize, hasher);
    }
    return hasNextBlock;
}

void FileComparer::filesToPaths(const std::list<GroupOfFiles>& files, std::list<GroupOfPaths>& target) const {
    for (auto& group : files) {
        GroupOfPaths filepaths;
        for (auto& file : group) {
            filepaths.push_back(file->getFilepath());
        }
        target.push_back(filepaths);
    }
}

std::list<FileComparer::GroupOfPaths> FileComparer::findDuplicateFiles(std::vector<std::unique_ptr<File>>& target) const {
    //group files by sizes
    std::multimap<std::size_t, std::unique_ptr<File>> fileBySize;
    for (auto& file : target) {
        fileBySize.emplace(file->getFileSize(), std::move(file)); //ATTENTION: the file is MOVED thus the original container becomes invalid
    }

    //remove all files with a unique size
    auto it = fileBySize.begin();
    while (it != fileBySize.end()) {
        if (fileBySize.count(it->second->getFileSize()) == 1) {
            it = fileBySize.erase(it);
        } else {
            ++it;
        }
    }

    //find duplicate files
    std::list<GroupOfPaths> filepaths;
    for (auto sizeIt = fileBySize.begin(); sizeIt != fileBySize.end(); sizeIt = fileBySize.upper_bound(sizeIt->first)) {
        auto group = fileBySize.equal_range(sizeIt->first);
        GroupOfFiles files;
        for (auto groupIt = group.first; groupIt != group.second; ++groupIt) {
            files.push_back(std::move(groupIt->second));
        }
        findAndAppendDuplicates(files, filepaths);
    }
    return filepaths;
}

};
