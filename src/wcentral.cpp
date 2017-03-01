#include "wcentral.h"

#include <QtGui>
#include <QtWidgets>
#include "analogclock.h"

#include "wrobotichand.h"
#include "wcontrol.h"
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

    m_wRoboticHand = new WRoboticHand(this);

    m_wControl = new WControl(this);

    AnalogClock *analogClock = new AnalogClock(this);
    analogClock->setUseAntialiasing(true);
    analogClock->colorHours(Qt::red);
    analogClock->colorMinutes(Qt::yellow);
    analogClock->colorHoursPointer(QColor(21, 255, 0, 60));
    analogClock->colorMinutesPointer(QColor(0, 199, 255, 60));

    /*
     * Laydown widgets
     */

    QVBoxLayout *layoutMain = new QVBoxLayout;
    QHBoxLayout *layoutVisualizer = new QHBoxLayout;
    QHBoxLayout *layoutControl = new QHBoxLayout;

    layoutVisualizer->addWidget(m_wRoboticHand);
    layoutVisualizer->addWidget(analogClock);

    layoutControl->addWidget(m_wControl);
    layoutControl->addStretch(1);

    layoutMain->addLayout(layoutVisualizer);
    layoutMain->addStretch(1);
    layoutMain->addLayout(layoutControl);

    setLayout(layoutMain);
}

void WCentral::connect(const QString &address)
{
}
