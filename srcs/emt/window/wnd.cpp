#include "wnd.h"
#include "window_config.h"

namespace emt
{
wnd::wnd(wnd *parent)
{
}

wnd::wnd(wnd *parent, uint x, uint y, uint cx, uint cy, const char *text)
{
    m_rect = {x, y, cx, cy};
}

wnd::~wnd()
{
    if (m_hwnd)
        DestroyWindow(m_hwnd);
    // TODO remove class name from static
}

void wnd::create()
{
    // create window properties
    WNDCLASSEXA wc{};
    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hInstance = GetModuleHandleA(nullptr);
    wc.lpszClassName = "win32++";
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hIcon = nullptr;

    ATOM res = RegisterClassExA(&wc);

    // TODO CS for static passing to args
    CREATESTRUCTA cs{};
    cs.lpszClass = wc.lpszClassName;
    cs.hInstance = wc.hInstance;
    cs.hwndParent = m_parent ? m_parent->m_hwnd : nullptr; // has parent
    cs.hMenu = 0;
    cs.lpszClass = wc.lpszClassName;
    cs.dwExStyle = NULL;
    cs.lpszName = m_text.c_str();
    cs.x = m_rect.x;
    cs.y = m_rect.y;
    cs.cx = m_rect.width;
    cs.cy = m_rect.height;

    m_hwnd = CreateWindowExA(
        cs.dwExStyle,
        cs.lpszClass,
        cs.lpszName,
        cs.style,
        cs.x, cs.y, cs.cx, cs.cy,
        cs.hwndParent,
        cs.hMenu,
        cs.hInstance, this); // custom pointer "this == Wnd"
}

LRESULT wnd::global_wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    // TODO get per object
    // Wnd* currentWnd = from lp

    // let them passing to default window event queue
    return ::DefWindowProc(hwnd, msg, wp, lp);
}

void wnd::show()
{
    if (m_hwnd)
        ::ShowWindow(m_hwnd, SW_SHOW);
}

} // namespace emt
