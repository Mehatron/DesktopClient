#ifndef _W_CENTRAL_
#define _W_CENTRAL_

#include <QWidget>

#include "robotichandcore.h"

class WRoboticHand;
class WControl;
class Joypad;

class WCentral : public QWidget
{
    Q_OBJECT

public:
    explicit WCentral(QWidget *parent = 0);
    virtual ~WCentral(void);

private:
    void setupGUI(void);

    WRoboticHand *m_wRoboticHand;
    WControl *m_wControl;
    Joypad *m_joypad;

public slots:
    void connect(const QString &address);
};

#endif // _W_CENTRAL_
