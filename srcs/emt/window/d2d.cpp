#include "d2d.h"
#include <d2d1.h>
#include <dwrite.h>
#include <assert.h>

namespace emt
{
void d2d::initialize()
{
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2d_factory);

    res = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&dwrite_factory);

    dwrite_factory->CreateTextFormat(
        L"Segoe UI",
        nullptr,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        42.0f,
        L"",
        &default_font);
    dwrite_factory->CreateTextFormat(
        L"Segoe UI",
        nullptr,
        DWRITE_FONT_WEIGHT_THIN,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        18.0f,
        L"en-us",
        &default_font1);
}

void d2d::release()
{
    safe_release(d2d_factory);
    safe_release(dwrite_factory);
    CoUninitialize();
}
HRESULT d2d::create_d2d_render_taget(
    HWND hwnd,
    ID2D1HwndRenderTarget** pp_render_target,
    ID2D1SolidColorBrush** pp_brush)
{
    RECT rc{};
    GetClientRect(hwnd, &rc);
    const D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

    D2D1_HWND_RENDER_TARGET_PROPERTIES hwnd_props =
        D2D1::HwndRenderTargetProperties(hwnd, size, D2D1_PRESENT_OPTIONS_IMMEDIATELY);

    D2D1_RENDER_TARGET_PROPERTIES render_target_props =
        D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
            96.f, 96.f);

    ID2D1HwndRenderTarget* render_target{};
    HRESULT hr = emt::d2d::d2d_factory->CreateHwndRenderTarget(render_target_props, hwnd_props, &render_target);

    *pp_render_target = render_target;

    ID2D1SolidColorBrush* brush{};

    hr = render_target->CreateSolidColorBrush({1.f, 1.f, 1.f, 1.f}, &brush);

    *pp_brush = brush;

    return hr;
}

// transform operator
D2D1_RECT_F to_d2d_rectf(const rect& rc)
{
    return {(float)rc.x, (float)rc.y, (float)(rc.cx + rc.x), (float)(rc.cy + rc.y)};
}

d2d_painter::d2d_painter(
    ID2D1HwndRenderTarget* target,
    ID2D1SolidColorBrush* brush)
    : target(target), cur_brush(brush)
{
    assert(target);
    assert(brush);

    target->BeginDraw();
    // m_target->Clear({0.3, 0.3, 0.3, 1});
    // m_brush->SetColor({.94, .94, .94, 1});
    // // m_target->DrawRectangle({10, 10, 100, 100}, m_brush, 0.7f);
    // m_target->DrawRoundedRectangle({30, 30, 300, 300, 10, 10}, m_brush);
    // d2d::default_font1->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    // m_target->DrawTextA(L"Painter of D2D1 한국", 20, d2d::default_font1, {0, 0, 300, 40}, m_brush);
}
d2d_painter::~d2d_painter()
{
    target->EndDraw();
}
void d2d_painter::clear_color(const colorf& color)
{
    target->Clear({color.r, color.g, color.b, color.a});
}
void d2d_painter::set_brush_color(const colorf& color)
{
    cur_brush->SetColor({color.r, color.g, color.b, color.a});
}
void d2d_painter::draw_rectangle(const rect& rc)
{
    target->DrawRectangle(to_d2d_rectf(rc), cur_brush);
}
} // namespace emt
