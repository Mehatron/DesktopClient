#include "wcentral.h"

#include <QtGui>
#include <QGridLayout>
#include "analogclock.h"

#include "wrobotichand.h"
#include "joypad.h"

WCentral::WCentral(QWidget *parent)
    : QWidget(parent)
{
    m_joypad = new Joypad(this);
    setupGUI();
}

WCentral::~WCentral(void)
{
}

void WCentral::setupGUI(void)
{
    /*
     * Create widgets
     */

    QGridLayout *layoutMain = new QGridLayout;

    m_wRoboticHand = new WRoboticHand(this);

    AnalogClock *analogClock = new AnalogClock(this);
    analogClock->setUseAntialiasing(true);
    analogClock->colorHours(Qt::red);
    analogClock->colorMinutes(Qt::yellow);
    analogClock->colorHoursPointer(QColor(21, 255, 0, 60));
    analogClock->colorMinutesPointer(QColor(0, 199, 255, 60));

    /*
     * Laydown widgets
     */

    layoutMain->addWidget(m_wRoboticHand, 0, 0);
    layoutMain->addWidget(analogClock, 0, 1);

    setLayout(layoutMain);
}

void WCentral::connect(const QString &address)
{
}
