#include "painter.h"

namespace emt
{
painter::painter(HWND hnwd) : m_hwnd(hnwd)
{
    ::BeginPaint(m_hwnd, &ps);
}

painter::painter(HWND hwnd, HDC dc)
    : m_hwnd(hwnd), m_dc(dc), owns_paint(false)
{
    // using the external dc
}

void painter::draw_rect(const RECT &rc, HBRUSH hr)
{
    ::FillRect(m_dc, &rc, hr);
}

} // namespace emt
