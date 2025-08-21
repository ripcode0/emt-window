#pragma once

#include <emt/core/typedef.h>
#include <string> // TODO < remove string

namespace emt
{
class wnd
{
public:
    wnd(wnd* parent);
    wnd(wnd* parent, uint x, uint y, uint cx, uint cy, const char* text);
    virtual ~wnd();

    // TODO : create func

    void create();

    void show();
    const char* get_text() const;

protected:
    /// @brief Called before the window class is registered with RegisterClassExA.
    ///        Allows modification of the WNDCLASSEXA structure (styles, icons, cursor, etc.)
    ///        before registration.
    /// @param wc Pointer to the WNDCLASSEXA structure to be filled or adjusted.
    virtual void pre_register(WNDCLASSEXA* wc);
    virtual void pre_create(CREATESTRUCT* cs);

    /// @brief Handles WM_NCCREATE before the client area is created.
    ///        Commonly used to store the 'this' pointer with GWLP_USERDATA.
    /// @param cs Pointer to CREATESTRUCT containing creation parameters.
    /// @return TRUE to continue creation, FALSE to abort.
    virtual LRESULT on_nccreate(CREATESTRUCT* cs);
    /// @brief When CreateWindowEx is called, the system sends WM_NCCCREATE first then WM_CREATE
    /// @param cs Pointer to a CREATESTRUCT
    /// @return 0 : success (wiindow creation continue.) -1 : failure CreateWindowEx returns NULL
    virtual LRESULT on_create(CREATESTRUCT* cs);

    virtual LRESULT on_paint(painter* painter);
    virtual LRESULT on_resize(size_event* evt);

    virtual LRESULT WINAPI local_wnd_proc(UINT msg, WPARAM wp, LPARAM lp);

    static LRESULT WINAPI global_wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

    wnd* m_parent{};
    rect m_rect{};
    HWND m_hwnd{};
    HBRUSH m_brush{};
    HFONT m_font{};
    std::string m_text;

    bool m_enable_paint = false;
    struct
    {
        bool d2d_enalble;
    } props;
};
} // namespace emt
