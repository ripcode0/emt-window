#pragma once

#include "../wnd.h"

namespace emt
{
class window : public wnd
{
public:
    window(wnd *parent, uint x, uint y, uint cx, uint cy, const char *text);
};
} // namespace emt
