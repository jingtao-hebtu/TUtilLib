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
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <system_error>
#include <csignal>
#include <cctype>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cerrno>
#include <pwd.h>
#include <sys/socket.h>
#include <netinet/in.h>


namespace fs = std::filesystem;


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

    bool createDir(const std::string &dir_path) {
        if (dir_path.empty()) {
            return false;
        }

        std::error_code ec;
        const auto path = std::filesystem::path(dir_path);
        if (std::filesystem::exists(path, ec)) {
            if (ec) {
                return false;
            }
            return std::filesystem::is_directory(path, ec);
        }

        bool created = std::filesystem::create_directories(path, ec);
        if (ec) {
            return false;
        }

        std::error_code verify_ec;
        if (std::filesystem::exists(path, verify_ec)) {
            return true;
        }

        return created && !verify_ec;
    }

    std::string getCurWorkDir() {
        std::array<char, kPathBufferSize> buff{};
        if (getcwd(buff.data(), buff.size()) == nullptr) {
            throw std::runtime_error("Failed to retrieve current working directory.");
        }
        return std::string(buff.data());
    }

    std::string joinPath(const std::vector<std::string> &paths) {
        fs::path joined_path;
        bool has_component = false;

        for (const std::string &ele: paths) {
            if (ele.empty()) {
                continue;
            }

            if (!has_component) {
                joined_path = fs::path(ele);
                has_component = true;
            } else {
                joined_path /= fs::path(ele);
            }
        }

        joined_path.make_preferred();
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
        if (target_process_name == nullptr || target_process_name[0] == '\0') {
            return;
        }

        std::error_code ec;
        for (const auto &entry : std::filesystem::directory_iterator("/proc", ec)) {
            if (ec) {
                break;
            }

            if (!entry.is_directory(ec)) {
                if (ec) {
                    ec.clear();
                }
                continue;
            }

            const std::string pid_str = entry.path().filename().string();
            if (!std::all_of(pid_str.begin(), pid_str.end(), [](unsigned char ch) {
                return std::isdigit(static_cast<int>(ch)) != 0;
            })) {
                continue;
            }

            std::ifstream comm_file(entry.path() / "comm");
            if (!comm_file.is_open()) {
                continue;
            }

            std::string process_name;
            std::getline(comm_file, process_name);
            if (process_name == target_process_name) {
                pid_t pid = static_cast<pid_t>(std::stoi(pid_str));
                ::kill(pid, SIGKILL);
            }
        }
    }

    int getValidPort() {
        int sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            return 0;
        }

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = 0;

        if (::bind(sockfd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
            ::close(sockfd);
            return 0;
        }

        socklen_t len = sizeof(addr);
        if (::getsockname(sockfd, reinterpret_cast<sockaddr*>(&addr), &len) == -1) {
            ::close(sockfd);
            return 0;
        }

        int port = ntohs(addr.sin_port);
        ::close(sockfd);
        return port;
    }

}
