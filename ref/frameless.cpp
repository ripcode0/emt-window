// EMT Window - frameless custom titlebar single-file example
// - Unicode (Wide) API 사용
// - Win11 다크모드/둥근모서리 선택적 적용
// - 드래그/리사이즈/더블클릭 최대화/시스템메뉴/버튼(-,□,X) 처리
// - DPI 대응(Title/Border/버튼 스케일)
//
// 빌드: cl /std:c++20 /DUNICODE /D_UNICODE /Zc:preprocessor /EHsc emt_wnd.cpp user32.lib gdi32.lib dwmapi.lib
// 또는: clang-cl /std:c++20 /DUNICODE /D_UNICODE emt_wnd.cpp user32.lib gdi32.lib dwmapi.lib
// 데모 실행: /DEMT_WND_DEMO 정의하여 WinMain 포함

#define UNICODE
#define _UNICODE

#include <algorithm>
#include <cstdint>
#include <dwmapi.h>
#include <string>
#include <string_view>
#include <uxtheme.h>
#include <windows.h>
#include <windowsx.h>

#pragma comment(lib, "dwmapi.lib")

namespace emt
{

struct Rect
{
    int x{}, y{}, w{}, h{};
};

// 유틸: DPI 헬퍼
static UINT GetDpiForWindowSafe(HWND hwnd)
{
    if (auto pGetDpi = reinterpret_cast<UINT(WINAPI *)(HWND)>(GetProcAddress(GetModuleHandleW(L"user32"), "GetDpiForWindow")))
        return pGetDpi(hwnd);
    HDC hdc = GetDC(hwnd);
    UINT dpi = (UINT)(hdc ? GetDeviceCaps(hdc, LOGPIXELSX) : 96);
    if (hdc)
        ReleaseDC(hwnd, hdc);
    return dpi ? dpi : 96;
}

static int ScaleByDpi(int px, UINT dpi) { return MulDiv(px, dpi, 96); }

class wnd
{
public:
    explicit wnd(wnd *parent = nullptr) noexcept : parent_(parent) {}

    wnd(wnd *parent, int x, int y, int cx, int cy, std::wstring_view title)
        : parent_(parent) { create(x, y, cx, cy, title); }

    ~wnd() { destroy(); }

    wnd(const wnd &) = delete;
    wnd &operator=(const wnd &) = delete;
    wnd(wnd &&o) noexcept { move_from(o); }
    wnd &operator=(wnd &&o) noexcept
    {
        if (this != &o)
        {
            destroy();
            move_from(o);
        }
        return *this;
    }

    bool create(int x, int y, int cx, int cy, std::wstring_view title)
    {
        ensure_class_registered();

        DWORD style = WS_POPUP | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;
        DWORD ex = WS_EX_APPWINDOW;

        hwnd_ = CreateWindowExW(
            ex, kClassName, title.data(), style,
            x, y, cx, cy,
            parent_ ? parent_->hwnd_ : nullptr,
            nullptr, GetModuleHandleW(nullptr), this);

        if (!hwnd_)
            return false;

        apply_dwm_attributes();
        UpdateWindow(hwnd_);
        return true;
    }

    void destroy() noexcept
    {
        if (hwnd_)
        {
            DestroyWindow(hwnd_);
            hwnd_ = nullptr;
        }
    }

    HWND native_handle() const noexcept { return hwnd_; }

private:
    // -------- 커스텀 타이틀바 파라미터 --------
    int title_px(UINT dpi) const { return ScaleByDpi(32, dpi); }
    int border_px(UINT dpi) const { return ScaleByDpi(8, dpi); }

    RECT rc_buttons(UINT dpi) const
    {
        RECT rc{};
        GetClientRect(hwnd_, &rc);
        int h = title_px(dpi);
        int bw = h; // 정사각형 버튼 3개
        rc.top = 0;
        rc.bottom = h;
        rc.right = rc.right;
        rc.left = std::max(0, (int)(rc.right - 3 * bw));

        return rc;
    }

    RECT rc_btn_close(UINT dpi) const
    {
        RECT r = rc_buttons(dpi);
        int bw = r.bottom - r.top;
        return RECT{r.right - bw, r.top, r.right, r.bottom};
    }
    RECT rc_btn_max(UINT dpi) const
    {
        RECT r = rc_buttons(dpi);
        int bw = r.bottom - r.top;
        return RECT{r.right - 2 * bw, r.top, r.right - bw, r.bottom};
    }
    RECT rc_btn_min(UINT dpi) const
    {
        RECT r = rc_buttons(dpi);
        int bw = r.bottom - r.top;
        return RECT{r.right - 3 * bw, r.top, r.right - 2 * bw, r.bottom};
    }

    // -------- GDI 그리기 --------
    void paint_titlebar(HDC hdc)
    {
        UINT dpi = GetDpiForWindowSafe(hwnd_);
        RECT rc;
        GetClientRect(hwnd_, &rc);
        int h = title_px(dpi);

        HBRUSH brTitle = CreateSolidBrush(RGB(30, 30, 30));
        HBRUSH brBG = CreateSolidBrush(RGB(45, 45, 45));
        const RECT rc0 = {0, 0, rc.right, h};
        const RECT rc1 = {0, h, rc.right, rc.bottom};
        FillRect(hdc, &rc0, brTitle);
        FillRect(hdc, &rc1, brBG);
        DeleteObject(brTitle);
        DeleteObject(brBG);

        // 제목 텍스트
        std::wstring title(256, L'\0');
        GetWindowTextW(hwnd_, title.data(), (int)title.size());
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(230, 230, 230));
        RECT rcText{ScaleByDpi(10, dpi), 0, rc.right, h};
        DrawTextW(hdc, title.c_str(), -1, &rcText, DT_VCENTER | DT_SINGLELINE | DT_LEFT | DT_END_ELLIPSIS);

        // 버튼 윤곽
        auto draw_btn = [&](RECT r, bool hot)
        {
            HBRUSH br = CreateSolidBrush(hot ? RGB(70, 70, 70) : RGB(55, 55, 55));
            FillRect(hdc, &r, br);
            DeleteObject(br);
            FrameRect(hdc, &r, (HBRUSH)GetStockObject(BLACK_BRUSH));
        };
        POINT cur;
        GetCursorPos(&cur);
        ScreenToClient(hwnd_, &cur);
        RECT rMin = rc_btn_min(dpi), rMax = rc_btn_max(dpi), rClose = rc_btn_close(dpi);
        draw_btn(rMin, PtInRect(&rMin, cur));
        draw_btn(rMax, PtInRect(&rMax, cur));
        draw_btn(rClose, PtInRect(&rClose, cur));

        // 심볼 (단순)
        auto draw_line = [&](RECT r)
        { MoveToEx(hdc, r.left+6, r.bottom-8, nullptr); LineTo(hdc, r.right-6, r.bottom-8); };
        auto draw_box = [&](RECT r)
        { Rectangle(hdc, r.left + 6, r.top + 6, r.right - 6, r.bottom - 6); };
        auto draw_x = [&](RECT r)
        { MoveToEx(hdc, r.left+6, r.top+6, nullptr); LineTo(hdc, r.right-6, r.bottom-6); MoveToEx(hdc, r.right-6, r.top+6, nullptr); LineTo(hdc, r.left+6, r.bottom-6); };
        draw_line(rMin);
        draw_box(rMax);
        draw_x(rClose);
    }

    // -------- 메시지 처리 --------
    LRESULT wnd_proc(UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
        case WM_NCCALCSIZE:
            if (wParam)
                return 0; // 비클라이언트 제거
            break;

        case WM_NCHITTEST:
        {
            // 리사이즈/드래그 핫존 계산
            POINT pt{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            RECT wrc;
            GetWindowRect(hwnd_, &wrc);
            UINT dpi = GetDpiForWindowSafe(hwnd_);
            int B = border_px(dpi);

            const bool left = pt.x < wrc.left + B;
            const bool right = pt.x >= wrc.right - B;
            const bool top = pt.y < wrc.top + B;
            const bool bottom = pt.y >= wrc.bottom - B;
            if (top && left)
                return HTTOPLEFT;
            if (top && right)
                return HTTOPRIGHT;
            if (bottom && left)
                return HTBOTTOMLEFT;
            if (bottom && right)
                return HTBOTTOMRIGHT;
            if (left)
                return HTLEFT;
            if (right)
                return HTRIGHT;
            if (top)
                return HTTOP;
            if (bottom)
                return HTBOTTOM;

            // 타이틀바 영역
            POINT cpt{pt};
            ScreenToClient(hwnd_, &cpt);
            if (cpt.y < title_px(dpi))
            {
                // 버튼 위는 드래그가 아닌 클릭이 되도록 HTCLIENT
                RECT rMin = rc_btn_min(dpi), rMax = rc_btn_max(dpi), rClose = rc_btn_close(dpi);
                if (PtInRect(&rMin, cpt) || PtInRect(&rMax, cpt) || PtInRect(&rClose, cpt))
                    return HTCLIENT;
                return HTCAPTION;
            }
            return HTCLIENT;
        }

        case WM_LBUTTONDBLCLK:
        {
            UINT dpi = GetDpiForWindowSafe(hwnd_);
            POINT p{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            if (p.y < title_px(dpi))
            {
                PostMessageW(hwnd_, WM_SYSCOMMAND, IsZoomed(hwnd_) ? SC_RESTORE : SC_MAXIMIZE, 0);
                return 0;
            }
            break;
        }

        case WM_LBUTTONUP:
        {
            UINT dpi = GetDpiForWindowSafe(hwnd_);
            POINT p{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            RECT rMin = rc_btn_min(dpi), rMax = rc_btn_max(dpi), rClose = rc_btn_close(dpi);
            if (PtInRect(&rClose, p))
            {
                PostMessageW(hwnd_, WM_SYSCOMMAND, SC_CLOSE, 0);
                return 0;
            }
            if (PtInRect(&rMax, p))
            {
                PostMessageW(hwnd_, WM_SYSCOMMAND, IsZoomed(hwnd_) ? SC_RESTORE : SC_MAXIMIZE, 0);
                return 0;
            }
            if (PtInRect(&rMin, p))
            {
                PostMessageW(hwnd_, WM_SYSCOMMAND, SC_MINIMIZE, 0);
                return 0;
            }
            break;
        }

        case WM_RBUTTONUP:
        case WM_NCRBUTTONUP:
        {
            POINT pt;
            GetCursorPos(&pt);
            HMENU h = GetSystemMenu(hwnd_, FALSE);
            SetForegroundWindow(hwnd_);
            TrackPopupMenu(h, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd_, nullptr);
            return 0;
        }

        case WM_SYSKEYDOWN:
            if (wParam == VK_SPACE)
            {
                POINT pt;
                GetCursorPos(&pt);
                HMENU h = GetSystemMenu(hwnd_, FALSE);
                TrackPopupMenu(h, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd_, nullptr);
                return 0;
            }
            break;

        case WM_DPICHANGED:
        {
            auto newRect = reinterpret_cast<RECT *>(lParam);
            SetWindowPos(hwnd_, nullptr, newRect->left, newRect->top,
                         newRect->right - newRect->left, newRect->bottom - newRect->top,
                         SWP_NOZORDER | SWP_NOACTIVATE);
            return 0;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd_, &ps);
            paint_titlebar(hdc);
            EndPaint(hwnd_, &ps);
            return 0;
        }

        case WM_ERASEBKGND:
            return 1; // 깜빡임 감소
        }
        return DefWindowProcW(hwnd_, msg, wParam, lParam);
    }

    static LRESULT CALLBACK WndProcThunk(HWND h, UINT m, WPARAM w, LPARAM l)
    {
        wnd *self = nullptr;
        if (m == WM_NCCREATE)
        {
            auto cs = reinterpret_cast<CREATESTRUCTW *>(l);
            self = reinterpret_cast<wnd *>(cs->lpCreateParams);
            self->hwnd_ = h;
            SetWindowLongPtrW(h, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
        }
        else
        {
            self = reinterpret_cast<wnd *>(GetWindowLongPtrW(h, GWLP_USERDATA));
        }
        if (self)
            return self->wnd_proc(m, w, l);
        return DefWindowProcW(h, m, w, l);
    }

    static void ensure_class_registered()
    {
        static bool registered = false;
        if (registered)
            return;
        WNDCLASSEXW wc{sizeof(wc)};
        wc.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = &wnd::WndProcThunk;
        wc.hInstance = GetModuleHandleW(nullptr);
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.lpszClassName = kClassName;
        RegisterClassExW(&wc);
        registered = true;
    }

    void apply_dwm_attributes()
    {
        // Win11 다크모드/둥근모서리(가능 시)
        BOOL dark = TRUE;
        DwmSetWindowAttribute(hwnd_, 20 /*DWMWA_USE_IMMERSIVE_DARK_MODE*/, &dark, sizeof(dark));
        int pref = 2 /*DWMWCP_ROUND*/;
        DwmSetWindowAttribute(hwnd_, 33 /*DWMWA_WINDOW_CORNER_PREFERENCE*/, &pref, sizeof(pref));
    }

    void move_from(wnd &o) noexcept
    {
        parent_ = o.parent_;
        hwnd_ = o.hwnd_;
        o.hwnd_ = nullptr;
    }

private:
    inline static constexpr const wchar_t *kClassName = L"EMT_FramelessWndClass";

    wnd *parent_{};
    HWND hwnd_{};
};

} // namespace emt

// 간단 데모: 커스텀 타이틀바 창 생성
int main()
{
    emt::wnd w(nullptr);
    if (!w.create(CW_USEDEFAULT, CW_USEDEFAULT, 900, 600, L"EMT Frameless Window"))
        return 0;
    ShowWindow(w.native_handle(), SW_SHOW);

    MSG msg{};
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return (int)msg.wParam;
}
