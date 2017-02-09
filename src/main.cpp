#include <QApplication>

#include "wrobotichand.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    WRoboticHand widget;
    widget.show();

    return app.exec();
}
