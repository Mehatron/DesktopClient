#include  "wrobotichand.h"

#include <QtGui>

WRoboticHand::WRoboticHand(QWidget *parent)
    : QWidget(parent)
{
    connect(this, &WRoboticHand::shouldRepaint, [this]() {
        update();
    });
}

WRoboticHand::~WRoboticHand(void)
{
}

void WRoboticHand::setState(const RoboticHandCore::State &state)
{
    m_state = state;
    emit shouldRepaint();
}

void WRoboticHand::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    int x, y, w;
    x = 10;
    y = height() * 3 / 6;
    w = width() * 2 / 6;

    /*
     * Draw construction
     */
    if(!m_state.constructionDown)
        y -= height() / 6;
    if(m_state.constructionUp)
        y -= height() / 6;
    if(!m_state.left)
        w += width() / 6;
    if(m_state.right)
        w+= width() / 6;
    drawConstruction(p, x, y, w);

    /*
     * Calculate new possition
     */
    x = x + w - 12;
    y += 12;

    /*
     * Draw grab
     */
    drawGrab(p, x - 12, y + 12);

    /*
     * Draw rotation
     */
    drawRotation(p, x, y);
}

void WRoboticHand::drawConstruction(QPainter &p, int x, int y, int w)
{
    p.setBrush(Qt::green);
    p.drawRect(x, y, w, 25);
    p.setBrush(Qt::magenta);
    p.drawRect(x, y + 25, 25, height() - y - 25);
}

void WRoboticHand::drawRotation(QPainter &p, int x, int y)
{
    p.setBrush(Qt::red);
    p.drawEllipse(x - 50, y - 50, 100, 100);
}

void WRoboticHand::drawGrab(QPainter &p, int x, int y)
{
    int angle = 0;
    if(!m_state.rotationDown)
        angle += 45;
    if(m_state.rotationUp)
        angle += 45;

    p.translate(x, y);
    p.rotate(-angle);

    int h = height() / 10;
    if(!m_state.extendsUnextended)
        h += height() / 10;
    if(m_state.extendsExtended)
        h += height() / 10;
    p.setBrush(Qt::blue);
    p.drawRect(0, 0, 25, h);
    p.drawRect(-70, h - 5, 145, 5);
    p.drawRect(-70, h - 5, 5, 20);
    p.drawRect(70, h - 5, 5, 20);

    int w = 50;
    if(m_state.picked)
        w += 15;
    p.setBrush(Qt::yellow);
    p.drawRect(-75, h + 15, w, 15);
    p.drawRect(90 - w, h + 15, w, 15);
    p.drawRect(w - 75, h + 5, 10, 35);
    p.drawRect(90 - w, h + 5, 10, 35);
    p.rotate(angle);

    p.translate(-x, -y);
}
