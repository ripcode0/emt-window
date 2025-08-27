#pragma once

#include "wnd.h"

namespace emt
{
class subclass_control : public wnd
{
public:
    subclass_control(wnd* parent);
    subclass_control(wnd* parent, uint x, uint y, uint cx, uint cy, const char* text);
    virtual ~subclass_control();

    virtual void create() override;
    wnd_type get_type() const override;

    LRESULT local_def_wnd_proc(UINT msg, WPARAM wp, LPARAM lp) override;
    static LRESULT WINAPI subclass_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
                                        UINT_PTR id, DWORD_PTR data);
};
} // namespace emt
