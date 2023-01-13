#include "FileComparer.h"

#include <boost/bimap.hpp>
#include <boost/bimap/unordered_set_of.hpp>
#include <boost/bimap/multiset_of.hpp>

#include <deque>
#include <map>
#include <unordered_map>

namespace Homework {

auto FILE_BLOCK_COMPARATOR = [](const std::unique_ptr<File>& a, const std::unique_ptr<File>& b) {
    return *a < *b;
};

void findAndAppendDuplicates(std::list<std::unique_ptr<File>>& files, std::list<std::list<std::string>>& target) {
    std::deque<std::list<std::unique_ptr<File>>> unprocessedGroups;
    unprocessedGroups.push_back(std::move(files));

    std::list<std::list<std::unique_ptr<File>>> processedGroups;
    
    while (!unprocessedGroups.empty()) {
        std::list<std::unique_ptr<File>>& source = unprocessedGroups.front();

        bool hasNextBlock = true;
        while (hasNextBlock) {
            for (auto& file : source) {
                hasNextBlock = file->readNextBlock(); //we use '=' instead of '&=' because it is guaranteed that all files have the same size
            }

            source.sort(FILE_BLOCK_COMPARATOR);

            auto current = source.begin();
            auto next = source.begin();
            ++next; //it is guaranteed that there are at least two elements in the list

            std::size_t numberOfDuplicates = 0;

            bool mustRemoveLastElement = false;
            while (next != source.end()) {
                mustRemoveLastElement = false;

                auto& currentFilepath = (*current)->getFilepath();
                if (**current == **next) {
                    ++next;
                    ++current;
                    ++numberOfDuplicates;
                } else {
                    if (numberOfDuplicates == 0) {
                        ++next;
                        current = source.erase(current);
                        mustRemoveLastElement = true;
                    } else {
                        unprocessedGroups.emplace_back();
                        //move remaining files to the new group and break the loop
                        unprocessedGroups.back().splice(unprocessedGroups.back().end(), source, next, source.end());
                        numberOfDuplicates = 0;
                        break;
                    }
                }
            }

            if (mustRemoveLastElement) {
                source.erase(current);
            }
        }
        
        if (!unprocessedGroups.front().empty()) {
            processedGroups.push_back(std::move(unprocessedGroups.front()));
        }
        unprocessedGroups.erase(unprocessedGroups.begin());
    }

    for (auto& group : processedGroups) {
        std::list<std::string> filepaths;
        for (auto& file : group) {
            filepaths.push_back(file->getFilepath());
        }
        target.push_back(filepaths);
    }
}

std::list<std::list<std::string>> findDuplicateFiles(std::vector<std::unique_ptr<File>>& target) {
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
    std::list<std::list<std::string>> filepaths;
    for (auto sizeIt = fileBySize.begin(); sizeIt != fileBySize.end(); sizeIt = fileBySize.upper_bound(sizeIt->first)) {
        auto group = fileBySize.equal_range(sizeIt->first);
        std::list<std::unique_ptr<File>> files;
        for (auto groupIt = group.first; groupIt != group.second; ++groupIt) {
            files.push_back(std::move(groupIt->second));
        }
        findAndAppendDuplicates(files, filepaths);
    }
    return filepaths;
}

};
