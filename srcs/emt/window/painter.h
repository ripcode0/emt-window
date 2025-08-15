#pragma once

#include "window_config.h"

namespace emt
{
class painter
{
public:
    painter(HWND hnwd);
    painter(HWND hwnd, HDC dc);

    void draw_rect(const RECT &rc, HBRUSH hr);

    HWND m_hwnd;
    HDC m_dc;

private:
    PAINTSTRUCT ps{};
    bool owns_paint = false;
};
} // namespace emt
