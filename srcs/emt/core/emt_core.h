#pragma once

#define emt_aseert_msg

struct rectf
{
    inline float& operator[](int index)
    {
        return f[index];
    }
    union
    {
        float f[4];
        struct
        {
            float x, y, cx, cy;
        };
    };
};
