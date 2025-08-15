#include <emt/window/application.h>
#include <emt/window/gui/window.h>
#include <iostream>
#include <windows.h>

class sub_window : public emt::wnd
{
public:
    sub_window(emt::wnd *parent, const char *txt) : wnd(parent, 10, 10, 300, 300, txt)
    {
        create();
    }

protected:
    void pre_register(WNDCLASSEXA *wc) override
    {
        wc->lpszClassName = "sub++";
        wc->hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
    }

    void pre_create(CREATESTRUCT *cs) override
    {
        cs->style = WS_VISIBLE;
    }

    LRESULT on_nccreate(CREATESTRUCT *cs) override
    {
        return TRUE;
    }
};

using namespace emt;

int main(int args, char **argv)
{
    application app(args, argv);

    window main_window(nullptr, 500, 300, 600, 500, "my app");

    sub_window child(&main_window, "hello");

    main_window.show();

    auto exit_code = app.exec();

    return exit_code;
}