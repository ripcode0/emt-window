#pragma once

#include <emt/core/typedef.h>

namespace emt
{
template <typename T>
struct gdiobj
{
    virtual ~gdiobj();
};
template <typename T>
inline gdiobj<T>::~gdiobj()
{
}

} // namespace emt
