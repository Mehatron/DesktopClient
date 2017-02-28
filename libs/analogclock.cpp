#include "analogclock.h"

#include <math.h>
#include <QtGui>

AnalogClock::AnalogClock(QWidget *parent)
    : QWidget(parent),
      m_showSeconds(true),
      m_useAntialiasing(false),
      m_colorHours(Qt::red),
      m_colorMinutes(Qt::black),
      m_colorHoursPointer(Qt::blue),
      m_colorMinutesPointer(Qt::blue),
      m_colorSecondsPointer(Qt::red)
{
    setWindowTitle(tr("Analog Clock"));

    QTimer *timer = new QTimer(this);
    timer->start(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(redraw()));
}

AnalogClock::~AnalogClock(void)
{
}

void AnalogClock::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, m_useAntialiasing);
    QPen pen;

    /*
     * Draw times
     */

    for(int i = 0 ; i < 60; i++)
    {
        double angle = (double)(i * M_PI / 30.0);
        int r2 = m_r * 14/15;

        if(i % 5 == 0)
        {
            r2 -= (m_r - r2 ) * 4/6;
            pen.setColor(m_colorHours);
        } else
        {
            pen.setColor(m_colorMinutes);
        }

        int x1 = m_center.x() + m_r * cos(angle);
        int y1 = m_center.y() + m_r * sin(angle);
        int x2 = m_center.x() + r2 * cos(angle);
        int y2 = m_center.y() + r2 * sin(angle);

        painter.setPen(pen);
        painter.drawLine(x1, y1, x2, y2);
    }

    /*
     * Draw pointers
     */

    drawPointers(painter);

    event->accept();
}

void AnalogClock::drawPointers(QPainter &painter)
{
    QPen pen;
    QBrush brush;
    QTime time = QTime::currentTime();
    double angleHours, angleMinutes, angleSeconds;
    double angleBeutiful = 5 * M_PI / 6;
    int x1, x2, y1, y2;

    anglesFromTime(time, &angleHours, &angleMinutes, &angleSeconds);

    if(m_showSeconds)
    {
        int x1 = m_center.x();
        int y1 = m_center.y();
        int x2 = m_center.x() + m_r * cos(angleSeconds);
        int y2 = m_center.y() + m_r * sin(angleSeconds);

        painter.setPen(m_colorSecondsPointer);
        painter.drawLine(x1, y1, x2, y2);
    }

    QPolygon polygonMinutes;
    polygonMinutes << QPoint(m_center.x(), m_center.y())
                   << QPoint(m_center.x() + m_r / 10 * cos(angleMinutes + angleBeutiful),
                             m_center.y() + m_r / 10 * sin(angleMinutes + angleBeutiful))
                   << QPoint(m_center.x() + m_r * cos(angleMinutes),
                             m_center.y() + m_r * sin(angleMinutes))
                   << QPoint(m_center.x() + m_r / 10 * cos(angleMinutes - angleBeutiful),
                             m_center.y() + m_r / 10 * sin(angleMinutes - angleBeutiful))
                   << QPoint(m_center.x(), m_center.y());

    QPolygon polygonHours;
    polygonHours << QPoint(m_center.x(), m_center.y())
                 << QPoint(m_center.x() + m_r / 10 * cos(angleHours + angleBeutiful),
                           m_center.y() + m_r / 10 * sin(angleHours + angleBeutiful))
                 << QPoint(m_center.x() + m_r * cos(angleHours) * 2/3,
                           m_center.y() + m_r * sin(angleHours) * 2/3)
                 << QPoint(m_center.x() + m_r / 10 * cos(angleHours - angleBeutiful),
                           m_center.y() + m_r / 10 * sin(angleHours - angleBeutiful))
                 << QPoint(m_center.x(), m_center.y());

    painter.setPen(m_colorMinutesPointer);
    painter.setBrush(m_colorMinutesPointer);
    painter.drawPolygon(polygonMinutes);

    painter.setPen(m_colorHoursPointer);
    painter.setBrush(m_colorHoursPointer);
    painter.drawPolygon(polygonHours);
}

void AnalogClock::redraw(void)
{
    repaint();
}

QSize AnalogClock::sizeHint(void) const
{
    return QSize(100, 100);
}

void AnalogClock::anglesFromTime(const QTime &time, double *hours, double *minutes, double *seconds)
{
    double hoursAngle = (double)((time.hour() - 3) * M_PI / 6.0);
    double minutesAngle = (double)((time.minute() - 15) * M_PI / 30.0);
    double secondsAngle = (double)((time.second() - 15) * M_PI / 30.0);

    hoursAngle += (minutesAngle + M_PI / 2.0) / 12.0;
    minutesAngle += (secondsAngle + M_PI / 2.0) / 60.0;

    if(hours)
        *hours = hoursAngle;
    if(minutes)
        *minutes = minutesAngle;
    if(seconds)
        *seconds = secondsAngle;
}

void AnalogClock::resizeEvent(QResizeEvent *event)
{
    QSize newSize = event->size();

    m_center.setX(newSize.width() / 2);
    m_center.setY(newSize.height() / 2);
    m_r = qMin(newSize.width(), newSize.height()) / 2 - 10;

    event->accept();
}

