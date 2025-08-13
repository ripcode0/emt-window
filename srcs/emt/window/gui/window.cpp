#include "window.h"

namespace emt
{
window::window(wnd *parent, uint x, uint y, uint cx, uint cy, const char *text)
    : wnd(parent, x, y, cx, cy, text)
{
    // create func here derivered
    create();
}

} // namespace emt
