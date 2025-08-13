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

    // TEST static window procedule func
    static LRESULT WINAPI global_wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

    wnd *m_parent;
    rect m_rect{};
    HWND m_hwnd;
    std::string m_text;
};
} // namespace emt
