#pragma once

#include <emt/core/typedef.h>
#include "wnd_config.h"

namespace emt
{
class painter
{
public:
    painter(HWND hnwd);
    painter(HWND hwnd, HDC dc);
    ~painter();

    void draw_rect(const RECT& rc, HBRUSH hr);
    void draw_rectangle(const RECT& rc);
    void draw_round_rect(const RECT& rc, int rx, int ry, bool fill = false);
    void draw_text(const char* text, const RECT& rc, UINT flag);
    void set_pen(HPEN pen);
    void set_brush(HBRUSH brush);
    void set_text_color(const COLORREF& color);

    HWND m_hwnd;
    HDC m_dc;
    const RECT& get_rect() const { return ps.rcPaint; }

private:
    PAINTSTRUCT ps{};
    bool owns_paint = false;
};
} // namespace emt
