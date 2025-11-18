/**************************************************************************

           Copyright(C), tao.jing All rights reserved

 **************************************************************************
   File   : LIBDefine.h
   Author : tao.jing
   Date   : 2024/9/30
   Brief  :
**************************************************************************/
#ifndef TUTILLIB_TAOUTILDEFINE_H
#define TUTILLIB_TAOUTILDEFINE_H


#ifdef _WIN32
    #if defined(TAO_UTIL_STATIC)
        #define TAO_UTIL_API
    #elif defined(BUILDING_DLL)
        #define TAO_UTIL_API __declspec(dllexport)
    #else
        #define TAO_UTIL_API __declspec(dllimport)
    #endif
#else
    #define TAO_UTIL_API
#endif



#endif //TUTILLIB_TAOUTILDEFINE_H
