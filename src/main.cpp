#include <QApplication>

#include "wcentral.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    WCentral widget;
    widget.show();

    return app.exec();
}
