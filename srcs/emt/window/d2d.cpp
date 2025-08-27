#include "d2d.h"
#include <d2d1.h>
#include <dwrite.h>
#include <assert.h>
#include "wnd_config.h"

namespace emt
{
void d2d::initialize()
{
    HR(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED));
    HR(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2d_factory));

    HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&dwrite_factory));

    HR(dwrite_factory->CreateTextFormat(
        L"Segoe UI",
        nullptr,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        42.0f,
        L"",
        &default_font));

    HR(dwrite_factory->CreateTextFormat(
        L"Segoe UI",
        nullptr,
        DWRITE_FONT_WEIGHT_THIN,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        18.0f,
        L"en-us",
        &default_font1));
}

void d2d::release()
{
    safe_release(default_font);
    safe_release(default_font1);
    safe_release(d2d_factory);
    safe_release(dwrite_factory);
    CoUninitialize();
}
HRESULT d2d::create_d2d_render_context(
    HWND hwnd,
    d2d_render_context** pp_hwnd_renderer)
{
    if (!hwnd || !pp_hwnd_renderer)
        return E_INVALIDARG;
    *pp_hwnd_renderer = nullptr;

    RECT rc{};
    if (!::GetClientRect(hwnd, &rc))
    {
        return HRESULT_FROM_WIN32(GetLastError());
    }
    float dpi_x = 96.f;
    float dpi_y = 96.f;

    emt::d2d::d2d_factory->GetDesktopDpi(&dpi_x, &dpi_y);

    const D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

    D2D1_HWND_RENDER_TARGET_PROPERTIES hwnd_props =
        D2D1::HwndRenderTargetProperties(hwnd, size, D2D1_PRESENT_OPTIONS_IMMEDIATELY);

    D2D1_RENDER_TARGET_PROPERTIES render_target_props =
        D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_DEFAULT,
            // D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
            dpi_x, dpi_y);

    ID2D1HwndRenderTarget* target{};
    HRESULT hr = emt::d2d::d2d_factory->CreateHwndRenderTarget(render_target_props, hwnd_props, &target);
    if (FAILED(hr))
        return hr;

    ID2D1SolidColorBrush* brush{};
    hr = target->CreateSolidColorBrush({1.f, 1.f, 1.f, 1.f}, &brush);
    if (FAILED(hr))
    {
        safe_release(target);
        return hr;
    }

    d2d_render_context* renderer = new d2d_render_context();
    if (!renderer)
    {
        safe_release(target);
        safe_release(brush);
        return E_OUTOFMEMORY;
    }
    renderer->target = target;
    renderer->brush = brush;

    *pp_hwnd_renderer = renderer;

    target = nullptr;
    brush = nullptr;
    renderer = nullptr;

    return hr;
}

// d2d render context
d2d_render_context::~d2d_render_context()
{
    safe_release(target);
    safe_release(brush);
}

// transform operator
D2D1_RECT_F to_d2d_rectf(const rect& rc)
{
    return {(float)rc.x, (float)rc.y, (float)(rc.cx + rc.x), (float)(rc.cy + rc.y)};
}

D2D1_RECT_F to_d2d_rectf(const RECT& rc)
{
    return {(float)rc.left, (float)rc.top, (float)rc.right, (float)rc.bottom};
}

// d2d painter

d2d_painter::d2d_painter(d2d_render_context* context)
    : target(context->target), cur_brush(context->brush)
{
    assert(target);
    assert(cur_brush);
    target->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
    target->BeginDraw();
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
    target->DrawRectangle(to_d2d_rectf(rc), cur_brush, 1.0f);
}

void d2d_painter::set_antialias(bool antialias)
{
    if (antialias)
        target->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
    else
        target->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
}

void d2d_painter::draw_round_rect(const rect& rc, float dx, float dy)
{
    target->DrawRoundedRectangle({(float)rc.x, (float)rc.y, (float)rc.cx, (float)rc.cy, dx, dy}, cur_brush);
}

void d2d_painter::draw_fill_rect(const rect& rc)
{
    target->FillRectangle(to_d2d_rectf(rc), cur_brush);
}

void d2d_painter::draw_line(const pointf& p0, const pointf& p1)
{
    target->DrawLine({p0.x, p0.y}, {p1.x, p1.y}, cur_brush, 1.f);
}

} // namespace emt
