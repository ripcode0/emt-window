#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

// Control
#include <CommCtrl.h>
#pragma comment(linker, "\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define HR(x) __hr((x), #x, __FILE__, __LINE__)

inline void __hr(HRESULT hr, const char* expr, const char* file, int line)
{
    if (FAILED(hr))
    {
        char sysMsg[512] = {0};
        FormatMessageA(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            hr,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            sysMsg,
            sizeof(sysMsg),
            nullptr);

        const char* filename = strrchr(file, '\\');
        if (!filename)
            filename = strrchr(file, '/');
        filename = (filename ? filename + 1 : file);

        std::string errMsg;
        errMsg = "Expression: " + std::string(expr) + "\n" + "File: " + filename + "\n" + "Line: " + std::to_string(line) + "\n\n" + "HRESULT: 0x" + std::to_string(hr) + "\n" + sysMsg;

        MessageBoxA(nullptr, errMsg.c_str(), "HRESULT Failure", MB_ICONERROR | MB_OK);
        ExitProcess(static_cast<UINT>(hr));
    }
}

#ifdef _DEBUG
#define assert_msg(expr, fmt, ...)                                                 \
    do                                                                             \
    {                                                                              \
        if (!(expr))                                                               \
        {                                                                          \
            char buf[512];                                                         \
            sprintf_s(buf, "Assertion failed: %s\nFile: %s\nLine: %d\nMsg : " fmt, \
                      #expr, __FILE__, __LINE__, __VA_ARGS__);                     \
            MessageBoxA(nullptr, buf, "ASSERT (Debug)", MB_ICONERROR | MB_OK);     \
            abort();                                                               \
        }                                                                          \
    } while (0)
#else
#define assert_msg(expr, fmt, ...) ((void)0)
#endif
