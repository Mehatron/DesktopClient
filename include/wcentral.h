#ifndef _W_CENTRAL_
#define _W_CENTRAL_

#include <QWidget>
#include <QTcpSocket>

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

    void connect(const QString &address, int port);
    void disconnect(void);
    void openJoystick(const QString &device);

private:
    void setupGUI(void);
    void updateState(void);

    WRoboticHand *m_wRoboticHand;
    WControl *m_wControl;
    Joypad *m_joypad;

    QTcpSocket *m_socket;

    RoboticHandCore::State m_state;

private slots:
    void sendCommand(const QString &command);
    void readyRead(void);
};

#endif // _W_CENTRAL_
