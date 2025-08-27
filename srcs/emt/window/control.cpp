#include "control.h"
#include "wnd_config.h"

namespace emt
{
subclass_control::subclass_control(wnd* parent)
    : wnd(parent)
{
}

subclass_control::subclass_control(wnd* parent, uint x, uint y, uint cx, uint cy, const char* text)
    : wnd(parent, x, y, cx, cy, text)
{
}

subclass_control::~subclass_control()
{
    RemoveWindowSubclass(m_hwnd, subclass_control::subclass_proc, 0);
}

void subclass_control::create()
{
    wnd::create();
    SetWindowSubclass(m_hwnd, subclass_control::subclass_proc, (UINT_PTR)this, (DWORD_PTR)this);
    SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
}

wnd_type subclass_control::get_type() const
{
    return wnd_type::subclass;
}

LRESULT subclass_control::local_def_wnd_proc(UINT msg, WPARAM wp, LPARAM lp)
{
    return ::DefSubclassProc(m_hwnd, msg, wp, lp);
}

LRESULT subclass_control::subclass_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR id, DWORD_PTR data)
{
    subclass_control* control = (subclass_control*)(data);
    if (control)
    {
        return control->local_wnd_proc(msg, wp, lp);
    }
    return ::DefSubclassProc(hwnd, msg, wp, lp);
}

} // namespace emt
