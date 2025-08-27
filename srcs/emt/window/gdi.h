#pragma once

#include <emt/core/typedef.h>

namespace emt
{
template <typename T>
struct gdiobj
{
    virtual ~gdiobj();
    T get() { return static_cast<T>(obj); }
    operator T() { return static_cast<T>(obj); }
    HGDIOBJ obj;
};

struct gdi_pen : gdiobj<HPEN>
{
    gdi_pen(uint32 width, const colorf& color);
};

struct gdi_brush : gdiobj<HBRUSH>
{
    gdi_brush(const colorf& color);
};

} // namespace emt
