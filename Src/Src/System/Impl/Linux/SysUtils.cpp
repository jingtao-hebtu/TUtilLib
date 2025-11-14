/**************************************************************************

           Copyright(C), tao.jing All rights reserved

 **************************************************************************
   File   : SysUtils.cpp
   Author : tao.jing
   Date   : 2023/11/12
   Brief  : 
**************************************************************************/
#include "TSysUtils.h"

#include <array>
#include <sstream>
#include <stdexcept>
#include <ctime>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cerrno>
#include <pwd.h>


#define TAO_ACCESS(fileName, accessMode) access(fileName,accessMode)
#define MKDIR(path) mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)


namespace {
    constexpr std::size_t kPathBufferSize = 4096;
}

namespace TBase {

    std::string getUserName() {
        uid_t userid;
        struct passwd *pwd;
        userid = getuid();
        pwd = getpwuid(userid);
        return pwd->pw_name;
    }

    bool dirExists(const std::string &dir_path) {
        if (TAO_ACCESS(dir_path.c_str(), 0) == 0) {
            return true;
        } else {
            return false;
        }
    }

    std::string getCurWorkDir() {
        std::array<char, kPathBufferSize> buff{};
        if (getcwd(buff.data(), buff.size()) == nullptr) {
            throw std::runtime_error("Failed to retrieve current working directory.");
        }
        return std::string(buff.data());
    }

    std::string joinPath(const std::vector<std::string> &paths) {
        std::string path_sep = std::string("/");
        std::string joined_path;
        for (const std::string &ele: paths) {
            joined_path += ele;
            joined_path += path_sep;
        }
        return joined_path;
    }

    std::string getDate(char split_char) {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        std::ostringstream sstr;
        sstr << ltm->tm_year + 1900 << split_char;
        sstr << ltm->tm_mon + 1 << split_char;
        sstr << ltm->tm_mday;

        return sstr.str();
    }

    std::string getTime(char split_char) {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        std::ostringstream sstr;
        sstr << ltm->tm_hour << split_char;
        sstr << ltm->tm_min << split_char;
        sstr << ltm->tm_sec;

        return sstr.str();
    }

    std::string getDirFromFilePath(const std::string &file_path) {
        std::string dir;
        const size_t last_slash_idx = file_path.rfind('/');
        if (std::string::npos != last_slash_idx) {
            dir = file_path.substr(0, last_slash_idx);
        }
        return dir;
    }

    void killProcessByName(const char* target_process_name) {
        (void)target_process_name;
        throw std::runtime_error("Not implemented function.");
    }

    int getValidPort() {
        throw std::runtime_error("Not implemented function.");
    }

}
