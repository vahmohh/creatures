#include <QApplication>

#include "window/vwindow.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    VWindow window;

    window.show();

    return application.exec();
}
