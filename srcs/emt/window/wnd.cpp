#include "wnd.h"
#include <emt/window/wndow_config.h>

namespace emt
{
wnd::wnd(wnd *parent)
{
}

wnd::wnd(wnd *parent, uint x, uint y, uint cx, uint cy, const char *text)
{
}

void wnd::create()
{
    // create window properties
    WNDCLASSEXA wc{};
    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hInstance = GetModuleHandleA(nullptr);
    wc.lpszClassName = "win32++";
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hIcon = nullptr;

    CREATESTRUCTA cs{};
}

} // namespace emt
