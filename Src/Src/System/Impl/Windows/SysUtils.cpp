/**************************************************************************

           Copyright(C), tao.jing All rights reserved

 **************************************************************************
   File   : SysUtils.cpp
   Author : tao.jing
   Date   : 2023/11/12
   Brief  : 
**************************************************************************/
#include "TSysUtils.h"
#include "TMacros.h"

#include <ctime>
#include <io.h>
#include <direct.h>
#include <windows.h>
#include <Tlhelp32.h>
#include <comdef.h>


#define TAO_ACCESS(fileName, accessMode) _access(fileName,accessMode)
#define TAO_MKDIR(path) _mkdir(path)


namespace TBase {

    std::string getUserName() {
        const size_t MAX_LEN = 256;
        char szBuffer[MAX_LEN] = {0};
        DWORD dwUserNameSize = MAX_LEN;
        // GetUserNameA get ascii encoded user name, each element is a char
        GetUserNameA(reinterpret_cast<LPSTR>(szBuffer), &dwUserNameSize);
        return szBuffer;
    }

    bool dirExists(const std::string &dir_path) {
        if (TAO_ACCESS(dir_path.c_str(), 0) == 0) {
            return true;
        } else {
            return false;
        }
    }

    bool createDir(const std::string &dir_path) {
        unsigned dirPathLen = static_cast<unsigned long>(dir_path.length());
        if (dirPathLen > TAO_MAX_PATH_LEN) {
            return false;
        }
        char tmpDirPath[TAO_MAX_PATH_LEN] = {0};
        for (unsigned i = 0; i < dirPathLen; ++i) {
            tmpDirPath[i] = dir_path[i];
            if (tmpDirPath[i] == '\\' || tmpDirPath[i] == '/') {
                if (TAO_ACCESS(tmpDirPath, 0) != 0) {
                    int ret = TAO_MKDIR(tmpDirPath);
                    if (ret != 0) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    std::string getCurWorkDir() {
        char buff[TAO_MAX_PATH_LEN];
        _getcwd(buff, TAO_MAX_PATH_LEN);
        std::string cur_work_dir(buff);
        return cur_work_dir;
    }

    std::string joinPath(const std::vector<std::string> &paths) {
        std::string path_sep = std::string("\\");
        std::string joined_path;
        for (const std::string &ele: paths) {
            joined_path += ele;
            joined_path += path_sep;
        }
        return joined_path;
    }

    std::string getDate(char split_char) {
        time_t now = time(0);
        struct tm t_out;
        localtime_s(&t_out, &now);
        char date[128];
        strftime(date, sizeof(date), "%Y", &t_out);
        std::string year = date;
        strftime(date, sizeof(date), "%m", &t_out);
        std::string month = date;
        strftime(date, sizeof(date), "%d", &t_out);
        std::string day = date;

        std::string str_date = year + split_char + month
                               + split_char + day;
        return str_date;
    }

    std::string getTime(char split_char) {
        time_t now = time(0);
        struct tm t_out;
        localtime_s(&t_out, &now);
        char time[128];
        strftime(time, sizeof(time), "%H", &t_out);
        std::string hour = time;
        strftime(time, sizeof(time), "%M", &t_out);
        std::string minute = time;
        strftime(time, sizeof(time), "%S", &t_out);
        std::string second = time;

        std::string str_time = hour + split_char + minute
                               + split_char + second;
        return str_time;
    }

    std::string getDirFromFilePath(const std::string &file_path) {
        std::string dir;
        size_t last_backslash = file_path.rfind('\\');
        size_t last_slash = file_path.rfind('/');

        size_t last_slash_idx = std::string::npos;
        if (last_backslash != std::string::npos && last_slash != std::string::npos) {
            last_slash_idx = (last_backslash > last_slash) ? last_backslash : last_slash;
        } else if (last_backslash != std::string::npos) {
            last_slash_idx = last_backslash;
        } else if (last_slash != std::string::npos) {
            last_slash_idx = last_slash;
        }

        if (last_slash_idx != std::string::npos) {
            dir = file_path.substr(0, last_slash_idx + 1);
        }
        return dir;
    }

    void killProcessByName(const char* target_process_name) {
        HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
        PROCESSENTRY32 pEntry;
        pEntry.dwSize = sizeof(pEntry);
        BOOL hRes = Process32First(hSnapShot, &pEntry);
        while (hRes) {
            _bstr_t w_process_name(pEntry.szExeFile);
            const char* process_name = w_process_name;
            if (strcmp(process_name, target_process_name) == 0) {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
                                              (DWORD) pEntry.th32ProcessID);
                if (hProcess != NULL) {
                    TerminateProcess(hProcess, 9);
                    CloseHandle(hProcess);
                }
            }
            hRes = Process32Next(hSnapShot, &pEntry);
        }
        CloseHandle(hSnapShot);
    }

    int getValidPort() {
        SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (slisten == INVALID_SOCKET)
            return 0;

        int port = 0;
        sockaddr_in sin;
        sin.sin_port = htons(port);
        sin.sin_addr.s_addr = 0;
        sin.sin_addr.s_addr = INADDR_ANY;
        sin.sin_family = AF_INET;

        if (bind(slisten, (LPSOCKADDR) &sin, sizeof(struct sockaddr_in)) == SOCKET_ERROR) {
            return 0;
        }

        int len = sizeof(sin);
        if (getsockname(slisten, (struct sockaddr *) &sin, &len) != -1) {
            int port_ret = ntohs(sin.sin_port);
            closesocket(slisten);
            return port_ret;
        } else {
            closesocket(slisten);
            return 0;
        }
    }

}