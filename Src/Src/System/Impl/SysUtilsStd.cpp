/**************************************************************************

           Copyright(C), tao.jing All rights reserved

 **************************************************************************
   File   : SysUtilsStd.cpp
   Author : tao.jing
   Date   : 2023/11/25
   Brief  : 
**************************************************************************/
#include <algorithm>

#include "TSysUtils.h"
#include <utility>
#include <vector>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <iostream>


namespace fs = std::filesystem;


namespace TBase {

    bool fileExists(const std::string &file_name) {
        std::fstream _file;
        _file.open(file_name.c_str(), std::ios::in);
        if (!_file) {
            return false;
        } else {
            _file.close();
            return true;
        }
    }

    bool delDir(const std::string &dir_path) {
        std::error_code ec;
        bool success = std::filesystem::remove_all(dir_path, ec);
        if (ec) {
            std::cout << ec.message() <<std::endl;
        }
        return success;
    }

    std::string joinPath(const std::vector<std::string> &paths, const std::string &file_name) {
        std::string joined_path = joinPath(paths);
        return joined_path + file_name;
    }

    std::string joinPath(const std::string &path, const std::string &file_name) {
        std::vector<std::string> paths;
        paths.emplace_back(path);
        return joinPath(paths, file_name);
    }

    inline std::string normalizePath(std::string path) {
        std::replace(path.begin(), path.end(), '\\', '/');
        return path;
    }

    std::string searchFileInParentDirs(const std::string &file_name,
                                       bool &found,
                                       std::uint8_t search_folder_levels) {

        std::string file_path = normalizePath(file_name);

        fs::path rel = fs::path(file_path);
        fs::path cur = fs::current_path();
        std::string last_candidate;

        for (std::uint8_t level = 0; level < search_folder_levels; level++) {
            fs::path candidate = cur / rel;
            last_candidate = candidate.string();

            if (fs::exists(candidate)) {
                found = true;
                return last_candidate;
            }

            if (!cur.has_parent_path()) {
                break;
            }
            cur = cur.parent_path();
        }

        found = false;
        return last_candidate;
    }

}