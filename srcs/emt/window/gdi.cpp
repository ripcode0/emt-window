#include "gdi.h"
#include "wnd_config.h"

namespace emt
{
template <typename T>
inline gdiobj<T>::~gdiobj()
{
    printf("one\n");
    safe_delete_gdiobj(obj);
}

gdi_pen::gdi_pen(uint32 width, const colorf& color)
{
    obj = ::CreatePen(PS_SOLID, width, color);
}

gdi_brush::gdi_brush(const colorf& color)
{
    obj = ::CreateSolidBrush(color);
}

} // namespace emt
