#include "painter.h"
#include <assert.h>
namespace emt
{
painter::painter(HWND hnwd) : m_hwnd(hnwd)
{
    assert(hnwd);
    m_dc = ::BeginPaint(m_hwnd, &ps);
}

painter::painter(HWND hwnd, HDC dc)
    : m_hwnd(hwnd), m_dc(dc), owns_paint(false)
{
    // using the external dc
}

painter::~painter()
{
    ::EndPaint(m_hwnd, &ps);
}

void painter::draw_rect(const RECT& rc, HBRUSH hr)
{
    ::FillRect(m_dc, &rc, hr);
}

void painter::draw_rectangle(const RECT& rc)
{
    ::Rectangle(m_dc, rc.left, rc.top, rc.right, rc.bottom);
}

void painter::draw_round_rect(const RECT& rc, int rx, int ry, bool fill)
{
    int saved = ::SaveDC(m_dc);
    if (!fill)
    {
        ::SelectObject(m_dc, ::GetStockObject(NULL_BRUSH));
    }
    ::RoundRect(m_dc, rc.left, rc.top, rc.right, rc.bottom, rx, ry);
    RestoreDC(m_dc, saved);
}

void painter::draw_text(const char* text, const RECT& rc, UINT flag)
{
    SetBkMode(m_dc, TRANSPARENT);
    ::DrawTextA(m_dc, text, strlen(text), (LPRECT)&rc, flag);
}

void painter::set_pen(HPEN pen)
{
    ::SelectObject(m_dc, pen);
}

void painter::set_brush(HBRUSH brush)
{
    ::SelectObject(m_dc, brush);
}

void painter::set_text_color(const COLORREF& color)
{
    ::SetTextColor(m_dc, color);
}

} // namespace emt
