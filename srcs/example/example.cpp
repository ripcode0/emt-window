#include <emt/window/application.h>
#include <emt/window/gui/window.h>
#include <emt/window/painter.h>
#include <iostream>
#include <windows.h>

class sub_window : public emt::wnd
{
public:
    sub_window(emt::wnd* parent, const emt::rect& rc, const char* txt)
        : wnd(parent, rc.x, rc.y, rc.cx, rc.cy, txt)
    {
        m_enable_paint = true;
        create();
    }

protected:
    void pre_register(WNDCLASSEXA* wc) override
    {
        wc->lpszClassName = "sub++";
        wc->hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
    }

    void pre_create(CREATESTRUCT* cs) override
    {
        cs->style = WS_VISIBLE;
    }

    LRESULT on_nccreate(CREATESTRUCT* cs) override
    {
        return TRUE;
    }

    LRESULT on_paint(emt::painter* p) override
    {
        HBRUSH br = CreateSolidBrush(RGB(100, 50, 60));
        HPEN pen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
        p->set_pen(pen);
        p->set_brush(br);
        p->draw_round_rect({20, 20, 200, 200}, 10, 10);
        p->set_text_color(RGB(150, 160, 190));
        RECT rc = p->get_rect();
        p->draw_text(get_text(), rc, DT_CENTER);

        safe_delete_gdiobj(br);
        return 0;
    }

    LRESULT on_resize(emt::size_event* evt) override
    {
        printf("resize\n");
        //  printf("%s resized %d\n", get_text(), evt->state);
        return 0;
    }
};

#include <emt/window/d2d.h>

class d2d_window : public emt::wnd
{
public:
    d2d_window(emt::window* parent, const emt::rect& rc, const char* text)
        : wnd(parent, rc.x, rc.y, rc.cx, rc.cy, text)
    {
        m_enable_paint = true;
        create();

        emt::d2d::create_d2d_render_taget(m_hwnd, &g_target, &g_brush);

        int a = 0;
    }

protected:
    void pre_register(WNDCLASSEXA* wc) override
    {
        wc->lpszClassName = "d2d";
    }

    LRESULT on_paint(emt::painter* p) override
    {
        int a = 0;
        if (!g_target)
            return 0;
        emt::d2d_painter painter(g_target, g_brush);
        painter.clear_color(emt::colorf(0.2f, 0.2f, 0.3f));
        painter.set_brush_color({1, 0, 0});
        painter.draw_rectangle({10, 10, 200, 200});

        return 0;
    }

    inline static ID2D1HwndRenderTarget* g_target{};
    inline static ID2D1SolidColorBrush* g_brush{};
};

using namespace emt;

int main(int args, char** argv)
{
    application app(args, argv);

    window main_window(nullptr, 500, 150, 860, 720, "my app");

    rect sub_rc{20, 10, 360, 600};
    sub_window child(&main_window, sub_rc, "Painter of WinAPI GDI");
    rect d2d_rc{400, 10, 360, 600};
    d2d_window child2(&main_window, d2d_rc, "Painter of WinAPI GDI");
    child2.show();

    main_window.show();

    auto exit_code = app.exec();

    return exit_code;
}