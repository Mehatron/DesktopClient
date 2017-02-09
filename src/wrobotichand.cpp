#include  "wrobotichand.h"

#include <QtGui>

WRoboticHand::WRoboticHand(void)
{
}

WRoboticHand::~WRoboticHand(void)
{
}

void WRoboticHand::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    /*
     * Draw construction
     */
    p.setBrush(Qt::green);
    p.drawRect(10, height() / 2, width() * 2 / 3, 25);
}
