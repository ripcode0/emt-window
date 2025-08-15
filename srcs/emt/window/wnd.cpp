#include "wnd.h"
#include "window_config.h"

namespace emt
{
wnd::wnd(wnd *parent)
    : m_parent(parent), m_text("win32++")
{
    m_rect = {0, 0, 0, 0};
}

wnd::wnd(wnd *parent, uint x, uint y, uint cx, uint cy, const char *text)
    : m_parent(parent), m_text(text)
{
    m_rect = {x, y, cx, cy};
}

wnd::~wnd()
{
    safe_delete_gdiobj(m_brush);
    safe_delete_gdiobj(m_font);
    safe_delete_wnd(m_hwnd);
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
    wc.lpfnWndProc = wnd::global_wnd_proc;

    pre_register(&wc);

    BOOL is_exist = GetClassInfoExA(wc.hInstance, wc.lpszClassName, &wc);

    if (!is_exist)
    {
        SetLastError(0);
        ATOM res = RegisterClassExA(&wc);
        DWORD e = GetLastError();

        if (!res)
        {
            DebugBreak();
            OutputDebugStringA("failed to register class");
        }
    }

    HWND parent_hwnd{};
    if (m_parent && m_parent->m_hwnd)
    {
        parent_hwnd = m_parent->m_hwnd;
    }

    // TODO CS for static passing to args
    CREATESTRUCTA cs{};
    cs.lpszClass = wc.lpszClassName;
    cs.hInstance = wc.hInstance;
    cs.hwndParent = parent_hwnd;
    cs.hMenu = 0;
    cs.dwExStyle = NULL;
    cs.lpszName = m_text.c_str();
    cs.x = m_rect.x;
    cs.y = m_rect.y;
    cs.cx = m_rect.width;
    cs.cy = m_rect.height;
    cs.style = NULL;

    pre_create(&cs);

    if (m_parent)
    {
        cs.style &= ~WS_POPUP;
        cs.style |= WS_CHILD;
    }
    else
    {
        cs.style &= ~WS_CHILD;
        if ((cs.style & (WS_OVERLAPPEDWINDOW)) == 0)
        {
            cs.style |= WS_POPUP;
        }
    }

    // TODO hook pre create

    m_hwnd = CreateWindowExA(
        cs.dwExStyle,
        cs.lpszClass,
        cs.lpszName,
        cs.style,
        cs.x, cs.y, cs.cx, cs.cy,
        cs.hwndParent,
        cs.hMenu,
        cs.hInstance, this); // custom pointer "this == Wnd"

    if (!m_hwnd)
    {

        auto e = GetLastError();
        int a = 0;
    }
}

void wnd::pre_register(WNDCLASSEXA *wc)
{
    unused(wc);
}

void wnd::pre_create(CREATESTRUCT *cs)
{
    unused(cs);
}

LRESULT wnd::on_nccreate(CREATESTRUCT *cs)
{
    unused(cs);
    return TRUE;
}

LRESULT wnd::on_create(CREATESTRUCT *cs)
{
    unused(cs);
    return 0;
}

LRESULT wnd::local_wnd_proc(UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        LPCREATESTRUCT cs = reinterpret_cast<LPCREATESTRUCT>(lp);
        return on_create(cs);
    }

    default:
        break;
    }
    return DefWindowProc(m_hwnd, msg, wp, lp);
}

LRESULT wnd::global_wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    // returns nullptr if not registered
    emt::wnd *wnd = (emt::wnd *)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    if (WM_NCCREATE == msg)
    {
        LPCREATESTRUCT cs = reinterpret_cast<LPCREATESTRUCT>(lp);
        wnd = (emt::wnd *)cs->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)wnd);
        wnd->m_hwnd = hwnd;

        LRESULT res = wnd->on_nccreate(cs);
        return res;
    }

    if (wnd)
        return wnd->local_wnd_proc(msg, wp, lp);

    return ::DefWindowProc(hwnd, msg, wp, lp);
}

void wnd::show()
{
    if (m_hwnd)
        ::ShowWindow(m_hwnd, SW_SHOW);
}

} // namespace emt
