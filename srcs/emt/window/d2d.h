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

    static HRESULT create_d2d_render_taget(
        HWND hwnd,
        ID2D1HwndRenderTarget** pp_render_target,
        ID2D1SolidColorBrush** pp_brush);

    inline static IDWriteTextFormat* default_font{};
    inline static IDWriteTextFormat* default_font1{};
};

struct d2d_hwnd_renderer
{
    ID2D1HwndRenderTarget* target;
    ID2D1SolidColorBrush* brush;
};

struct d2d_painter
{
    d2d_painter(
        ID2D1HwndRenderTarget* target,
        ID2D1SolidColorBrush* brush);
    ~d2d_painter();

    void clear_color(const colorf& color);
    void set_brush_color(const colorf& color);
    void draw_rectangle(const rect& rc);

private:
    ID2D1HwndRenderTarget* target{};
    ID2D1SolidColorBrush* cur_brush{};
    HWND hwnd{};
};

} // namespace emt
