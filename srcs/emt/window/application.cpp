#include "application.h"
#include "window_config.h"

namespace emt
{
application::application(int args, char *argv[])
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
