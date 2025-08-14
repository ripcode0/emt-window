#include <emt/window/application.h>
#include <emt/window/gui/window.h>
#include <iostream>
#include <windows.h>

class sub_window : public emt::wnd
{
private:
    /* data */
public:
    sub_window(emt::wnd *parent, const char *txt) : wnd(parent, 10, 10, 300, 300, txt)
    {
        create();
    }

protected:
    void pre_register(WNDCLASSEXA *wc) override
    {
        wc->lpszClassName = "sub++";
        wc->hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    }

    void pre_create(CREATESTRUCT *cs) override
    {
        cs->style = WS_VISIBLE;
    }
};

using namespace emt;

int main(int args, char *argv[])
{
    // TODO application for windows
    application app;

    window main_window(nullptr, 100, 100, 600, 500, "my app");

    sub_window child(&main_window, "hello");

    main_window.show();

    auto exit_code = app.exec();

    return exit_code;
}