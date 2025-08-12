#pragma once

#include <emt/core/typedef.h>

namespace emt
{
class wnd
{
public:
    wnd(wnd *parent);
    wnd(wnd *parent, uint x, uint y, uint cx, uint cy, const char *text);

    wnd *m_parent;
    rect m_rect{};
    HWND m_hwnd;
};
} // namespace emt
