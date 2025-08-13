#include <emt/window/application.h>
#include <emt/window/wnd.h>
#include <iostream>

int main(int args, char *argv[])
{
    // TODO application for windows
    emt::application app;

    emt::wnd main_window(nullptr, 100, 100, 600, 500, "my app");

    auto exit_code = app.exec();

    return exit_code;
}