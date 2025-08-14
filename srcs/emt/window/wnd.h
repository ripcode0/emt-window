#pragma once

#include <emt/core/typedef.h>
#include <string> // TODO < remove string
namespace emt
{
class wnd
{
public:
    wnd(wnd *parent);
    wnd(wnd *parent, uint x, uint y, uint cx, uint cy, const char *text);
    virtual ~wnd();

    // TODO : create func

    void create();

    void show();

protected:
    virtual void pre_register(WNDCLASSEXA *wc);
    virtual void pre_create(CREATESTRUCT *cs);

    virtual LRESULT WINAPI local_wnd_proc(UINT msg, WPARAM wp, LPARAM lp);

    static LRESULT WINAPI global_wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

    wnd *m_parent{};
    rect m_rect{};
    HWND m_hwnd{};
    HBRUSH m_brush{};
    HFONT m_font{};
    std::string m_text;
};
} // namespace emt
