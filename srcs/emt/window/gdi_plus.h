#pragma once

#include "wnd_config.h"
#include <objidl.h>
#include <propidl.h>
#include <gdiplus.h>

namespace emt
{
struct gdi_plus
{
    inline static ULONG_PTR token{};
    inline static Gdiplus::GdiplusStartupInput input{};
    inline static void initialize()
    {
        if (Gdiplus::GdiplusStartup(&token, &input, nullptr) != Gdiplus::Ok)
        {
            token = 0;
        }
    }
};

} // namespace emt
