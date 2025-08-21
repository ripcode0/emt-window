#include "application.h"
#include "gdi_plus.h"
#include "wnd_config.h"
#include "d2d.h"

namespace emt
{
application::application(int args, char* argv[])
{
    gdi_plus::initialize();
    d2d::initialize();
}

application::~application()
{
}

int application::exec()
{
    // event messages
    MSG msg{};

    while (m_is_running)
    {
        while (GetMessageA(&msg, nullptr, 0, 0))
        {
            if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE)
            {
                PostQuitMessage(0);
                m_is_running = false;
            }
            ::TranslateMessage(&msg);
            ::DispatchMessageA(&msg);
        }
    }
    return 0; // return zero is normal
}

} // namespace emt
