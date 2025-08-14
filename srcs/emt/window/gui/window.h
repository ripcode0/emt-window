#pragma once

#include "../wnd.h"

namespace emt
{
class window : public wnd
{
public:
    window(wnd *parent, uint x, uint y, uint cx, uint cy, const char *text);

protected:
    void pre_register(WNDCLASSEXA *wc) override;
    void pre_create(CREATESTRUCT *cs) override;
};
} // namespace emt
