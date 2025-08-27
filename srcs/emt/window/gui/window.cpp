#include "window.h"
#include "../wnd_config.h"

namespace emt
{
window::window(wnd* parent, uint x, uint y, uint cx, uint cy, const char* text)
    : wnd(parent, x, y, cx, cy, text)
{
    // create func here derivered
    create();
}

void window::pre_register(WNDCLASSEXA* wc)
{
    wc->hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
}

void window::pre_create(CREATESTRUCT* cs)
{
    cs->style = WS_OVERLAPPEDWINDOW;
    printf("create struct from window\n");
}

} // namespace emt
