#include <emt/window/application.h>
#include <emt/window/gui/window.h>
#include <emt/window/painter.h>
#include <iostream>
#include <windows.h>
#include <emt/window/d2d.h>
#include <emt/window/gdi.h>

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
        // HPEN pen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
        emt::gdi_pen pen(1, {0.6, 0.55, 0.5f});

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

        HR(emt::d2d::create_d2d_render_context(m_hwnd, &d2d_context));
    }

protected:
    void pre_register(WNDCLASSEXA* wc) override
    {
        wc->lpszClassName = "d2d";
    }

    LRESULT on_paint(emt::painter* p) override
    {
        if (!d2d_context)
            return 0;
        emt::d2d_painter painter(d2d_context);
        painter.set_antialias(true);
        painter.clear_color(emt::colorf(0.141176f, 0.160784f, 0.180392f));
        painter.set_brush_color({0.2, 0.2, 0.2});
        // painter.draw_rectangle({10, 10, 200, 200});
        painter.draw_round_rect({50, 50, 200, 200}, 10, 10);
        painter.set_brush_color({0.113725f, 0.129412f, 0.145098f});
        painter.draw_fill_rect({10, 10, 200, 200});
        painter.set_brush_color({0.05, 0.05, 0.05});
        painter.set_antialias(true);
        painter.draw_line({0, 250}, {400, 250});

        painter.set_brush_color({1, 1, 0});
        const emt::rect rc = get_client_rect();

        painter.draw_round_rect(rc, 30, 30);

        return 0;
    }

    emt::d2d_render_context* d2d_context{};
};

using namespace emt;
#include <emt/window/control.h>

class button : public subclass_control
{
public:
    button(wnd* parent, uint x, uint y, uint cx, uint cy, const char* text)
        : subclass_control(parent, x, y, cx, cy, text)
    {
        m_class_name = WC_BUTTONA;
        m_enable_paint = true;
        create();
    }

    // LRESULT local_wnd_proc(UINT msg, WPARAM wp, LPARAM lp) override
    // {
    //     return DefSubclassProc(m_hwnd, msg, wp, lp);
    // }

    LRESULT on_paint(painter* p) override
    {
        gdi_brush br({0.2, 0.2, 0.2});
        p->draw_rect(p->get_rect(), br);
        printf("paint button %s\n", get_text());
        p->set_text_color(colorf{0.8, 0.8, 0.8});
        p->draw_text(get_text(), p->get_rect(), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        return 0;
    }
};

int main(int args, char** argv)
{
    application app(args, argv);

    window main_window(nullptr, 500, 150, 860, 720, "my app");

    rect sub_rc{20, 10, 360, 600};
    sub_window child(&main_window, sub_rc, "Painter of WinAPI GDI");
    rect d2d_rc{400, 10, 360, 600};
    // d2d_window child2(&main_window, d2d_rc, "Painter of WinAPI GDI");
    //  child2.show();

    button b(&main_window, 400, 0, 200, 100, "button");
    b.show();

    main_window.show();

    auto exit_code = app.exec();

    return exit_code;
}