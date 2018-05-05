//
// Copyright © 2018 Roman Sobkuliak <r.sobkuliak@gmail.com>
// This code is released under the license described in the LICENSE file
// 

#ifndef FOLDER_CRAWLER_H
#define FOLDER_CRAWLER_H

#include "ImageStore.h"

#include <regex>
#include <vector>
#include <string>
#include <filesystem>
#include <stack>
#include <memory>

#ifdef _MSC_VER     // MSVC compiler
    namespace fs = std::experimental::filesystem;
#else
    namespace fs = std::filesystem;
#endif // _MSC_VER


// FolderCrawler is used to crawl (potentially recursively) folders in folders_ vector and picks
// the files that should be filtered.
class FolderCrawler {
    bool recursive_;
    bool ram_friendly_;
    std::regex fname_regex_;
    std::string folder_prefix_;
    bool incl_folder_prefix_;

    std::stack< fs::path> folders_;

public:
    FolderCrawler(
        const std::vector< std::string> & folders,
        std::string folder_prefix,
        const std::string & fname_regex_str,
        bool incl_folder_prefix,
        bool recursive, 
        bool ram_friendly) 
        : recursive_(recursive), ram_friendly_(ram_friendly), fname_regex_(fname_regex_str),
         folder_prefix_(std::move(folder_prefix)), incl_folder_prefix_(incl_folder_prefix)

    {
        // Loop is reversed so that first folders in the original vector
        // will be first in the stack
        for (auto it = folders.crbegin(); it != folders.crend(); ++it)
            folders_.push(fs::path(*it));
    }

    // Inspects a single folder and returns IImageStore that contains
    // all files from the folder that matched fname_regex_
    std::unique_ptr< IImageStore> get_next_run();

    bool has_next_run() {
        return !folders_.empty();
    }
};

#endif // FOLDER_CRAWLER_H
