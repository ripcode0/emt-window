#pragma once

#include <emt/core/typedef.h>

namespace emt
{
struct d2d
{
    static void initialize();
    static void release();
    inline static ID2D1Factory* d2d_factory{};
    inline static IDWriteFactory* dwrite_factory{};

    static HRESULT create_d2d_render_context(
        HWND hwnd,
        d2d_render_context** pp_hwnd_renderer);

    inline static IDWriteTextFormat* default_font{};
    inline static IDWriteTextFormat* default_font1{};
};

struct d2d_render_context
{
    ID2D1HwndRenderTarget* target;
    ID2D1SolidColorBrush* brush;
    ~d2d_render_context();
};

struct d2d_painter
{
    d2d_painter(d2d_render_context* context);
    ~d2d_painter();

    void clear_color(const colorf& color);
    void set_brush_color(const colorf& color);
    void draw_rectangle(const rect& rc);
    void set_antialias(bool antialias);
    void draw_round_rect(const rect& rc, float dx, float dy);
    void draw_fill_rect(const rect& rc);
    void draw_line(const pointf& p0, const pointf& p1);

private:
    ID2D1HwndRenderTarget* target{};
    ID2D1SolidColorBrush* cur_brush{};
    HWND hwnd{};
};

} // namespace emt
