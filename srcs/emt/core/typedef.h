#pragma once

// clang-format off
#ifndef _INC_WINDOWS
    #ifndef CALLBACK
    #define CALLBACK __stdcall
#endif
#ifndef WINAPI
    #define WINAPI __stdcall
#endif
#ifndef APIENTRY
    #define APIENTRY WINAPI
#endif

typedef unsigned int    UINT;
typedef unsigned long   DWORD; // Windows(LLP64) 32-bit
typedef int             BOOL;
typedef unsigned char   BYTE;
typedef unsigned short  WORD;
typedef float           FLOAT;

typedef void            *LPVOID;
typedef const char      *LPCSTR;
typedef char            *LPSTR;

#if defined(_WIN64)
    #if defined(_MSC_VER)
        typedef __int64 INT_PTR;
        typedef unsigned __int64 UINT_PTR;
        typedef __int64 LONG_PTR;
        typedef unsigned __int64 ULONG_PTR;
    #else
        typedef long long INT_PTR;
        typedef unsigned long long UINT_PTR;
        typedef long long LONG_PTR;
        typedef unsigned long long ULONG_PTR;
    #endif
#else
    typedef int INT_PTR;
    typedef unsigned int UINT_PTR;
    typedef long LONG_PTR;
    typedef unsigned long ULONG_PTR;
#endif

typedef UINT_PTR WPARAM;
typedef LONG_PTR LPARAM;
typedef LONG_PTR LRESULT;
typedef long HRESULT;

typedef void *HANDLE;

struct HWND__;
typedef HWND__ *HWND;
struct HINSTANCE__;
typedef HINSTANCE__ *HINSTANCE;
struct HDC__;
typedef HDC__ *HDC;
struct HBRUSH__;
typedef HBRUSH__ *HBRUSH;
struct HGLRC__;
typedef HGLRC__ *HGLRC;
#endif

// clang-format on

#include <stdint.h>
typedef uint32_t uint, uint32;
typedef unsigned char byte, uint8;

// emt window type
namespace emt
{
struct rect
{
    uint32 x;
    uint32 y;
    uint32 width;
    uint32 height;
};

struct window_event
{
    UINT msg;
    WPARAM wp;
    LPARAM lp;
};

} // namespace emt
