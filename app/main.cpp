#include <QApplication>
#include "Window.h"

int main(int num_args, char** args)
{
    QApplication app(num_args, args);

    Window* win = new Window;
    win->show();

    const int ret = app.exec();

    delete win;

    return ret;
}
