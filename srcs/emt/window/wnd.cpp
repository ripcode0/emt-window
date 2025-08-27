#include "wnd.h"
#include "painter.h"
#include "wnd_config.h"
#include <assert.h>

namespace emt
{
wnd::wnd(wnd* parent)
    : m_parent(parent),
      m_text("win32++"),
      m_class_name("win32++")
{
    m_rect = {0, 0, 0, 0};
}

wnd::wnd(wnd* parent, uint x, uint y, uint cx, uint cy, const char* text)
    : m_parent(parent),
      m_text(text),
      m_class_name("win32++")
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
    wc.lpszClassName = m_class_name.c_str();
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hIcon = nullptr;
    wc.lpfnWndProc = wnd::global_wnd_proc;

    pre_register(&wc);

    BOOL is_exist = GetClassInfoExA(wc.hInstance, wc.lpszClassName, &wc);

    if (!is_exist)
    {
        assert_msg(RegisterClassExA(&wc), "failed to register %s class", wc.lpszClassName);
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
    cs.dwExStyle = 0;
    cs.lpszName = m_text.c_str();
    cs.x = m_rect.x;
    cs.y = m_rect.y;
    cs.cx = m_rect.cx;
    cs.cy = m_rect.cy;
    cs.style = 0;

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
        cs.hInstance, this);

    assert_msg(m_hwnd, "failed to create window");
}

void wnd::pre_register(WNDCLASSEXA* wc)
{
    unused(wc);
}

void wnd::pre_create(CREATESTRUCT* cs)
{
    unused(cs);
}

LRESULT wnd::on_nccreate(CREATESTRUCT* cs)
{
    unused(cs);
    return TRUE;
}

LRESULT wnd::on_create(CREATESTRUCT* cs)
{
    printf("ccc\n");
    unused(cs);
    return 0;
}

LRESULT wnd::on_paint(painter* painter)
{
    unused(painter);
    return 0;
}

LRESULT wnd::on_resize(size_event* evt)
{
    printf("hhh\n");
    unused(evt);
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
    case WM_PAINT:
    {
        printf("wm_paint %s\n", get_text());
        if (!m_enable_paint)
            break;
        painter p(m_hwnd);
        return on_paint(&p);
    }
    case WM_SIZE:
    {
        size_event evt{};
        evt.cx = static_cast<uint32_t>(LOWORD(lp));
        evt.cy = static_cast<uint32_t>(HIWORD(lp));
        evt.state = (emt::size_state)(wp);

        if (on_resize(&evt) == 0)
            return 0;
        break;
    }

    default:
        break;
    }
    return local_def_wnd_proc(msg, wp, lp);
}

LRESULT wnd::local_def_wnd_proc(UINT msg, WPARAM wp, LPARAM lp)
{
    return ::DefWindowProc(m_hwnd, msg, wp, lp);
}

LRESULT wnd::global_wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    // returns nullptr if not registered
    emt::wnd* wnd = (emt::wnd*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    if (WM_NCCREATE == msg)
    {
        LPCREATESTRUCT cs = reinterpret_cast<LPCREATESTRUCT>(lp);
        wnd = (emt::wnd*)cs->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)wnd);
        wnd->m_hwnd = hwnd;
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

const char* wnd::get_text() const
{
    thread_local static char buffer[1024];
    int len = ::GetWindowTextA(m_hwnd, buffer, sizeof(buffer));
    buffer[len] = '\0';
    return buffer;
}

wnd_type wnd::get_type() const
{
    return wnd_type::defualt;
}

bool wnd::is_control() const
{
    if (get_type() == wnd_type::defualt)
        return false;
    return true;
}

rect wnd::get_client_rect() const
{
    RECT rc{};
    ::GetClientRect(m_hwnd, &rc);

    return {(uint32)rc.left, (uint32)rc.top, ((uint32)rc.right - rc.left),
            ((uint32)rc.bottom - rc.top)};
}

} // namespace emt
