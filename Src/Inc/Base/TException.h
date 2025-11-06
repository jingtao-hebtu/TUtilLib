/**************************************************************************

           Copyright(C), tao.jing All rights reserved

 **************************************************************************
   File   : TException.h
   Author : tao.jing
   Date   : 2023/12/2
   Brief  : 
**************************************************************************/
#ifndef BREAKERMEA_TEXCEPTION_H
#define BREAKERMEA_TEXCEPTION_H

#include "TLog.h"
#include <cstdarg>
#include <exception>
#include <string>


#define TBASE_LOG_THROW_RUNTIME(...) \
{                              \
LOG_F(ERROR, __VA_ARGS__);     \
throw TBase::TBaseException(TBase::formatVarsToString(__VA_ARGS__)); \
}


namespace TBase {

    static inline std::string formatVarsToString(const char *fmt...) {
        char buf[512] = {0};
        std::va_list args;
                va_start(args, fmt);
        vsprintf_s(buf, fmt, args);
                va_end(args);
        return buf;
    }

    class TBaseException : public std::exception {

    public:
        explicit TBaseException(const char *msg) : std::exception(msg) {}

        explicit TBaseException(const std::string &msg) : std::exception(msg.c_str()) {}

    };

};


#endif //BREAKERMEA_TEXCEPTION_H
