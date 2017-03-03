#include "wcentral.h"

#include <QtGui>
#include <QtWidgets>
#include "json.hpp"
#include "analogclock.h"

#include "exception.h"
#include "wrobotichand.h"
#include "wcontrol.h"
#include "joypad.h"

WCentral::WCentral(QWidget *parent)
    : QWidget(parent),
      m_socket(nullptr)
{
    m_joypad = new Joypad(this);
    QObject::connect(m_joypad, &Joypad::sendCommand,
                     this, &WCentral::sendCommand);

    setupGUI();
}

WCentral::~WCentral(void)
{
    disconnect();
}

void WCentral::setupGUI(void)
{
    /*
     * Create widgets
     */

    m_wRoboticHand = new WRoboticHand(this);
    m_wRoboticHand->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_wControl = new WControl(this);
    QObject::connect(m_wControl, &WControl::sendCommand,
            this, &WCentral::sendCommand);

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
    //layoutMain->addStretch(1);
    layoutMain->addLayout(layoutControl);

    setLayout(layoutMain);
}

void WCentral::connect(const QString &address, int port)
{
    disconnect();

    m_socket = new QTcpSocket(this);

    m_socket->connectToHost(address, port);
    if(!m_socket->waitForConnected(1000))
        throw Exception("Connection error: " + m_socket->errorString().toStdString());
    QObject::connect(m_socket, &QTcpSocket::readyRead,
                     this, &WCentral::readyRead);
}

void WCentral::disconnect(void)
{
    if(m_socket)
        sendCommand("close");
}

void WCentral::openJoystick(const QString &device)
{
    bool success = true;

    try {
        m_joypad->open(device);
    } catch(Exception &ex) {
        qDebug() << ex.what();
        QMessageBox::warning(this, tr("Robotic Hand"),
                             tr("Can't open joystick: %1").arg(device));
    }
}

void WCentral::updateState(void)
{
    m_wControl->setState(m_state);
    m_wRoboticHand->setState(m_state);
}

void WCentral::sendCommand(const QString &command)
{
    if(!m_socket)
        return;

    QByteArray buffer = command.toLocal8Bit();
    for(int i = buffer.size(); i < 1024; i++)
        buffer.push_back('\0');
    m_socket->write(buffer);
}

void WCentral::readyRead(void)
{
    QString message = m_socket->readAll();
    nlohmann::json data = nlohmann::json::parse(message.toStdString());

    RoboticHandCore::State state;
    state.constructionDown = data["construction_down"];
    state.constructionUp = data["construction_up"];
    state.left = data["left"];
    state.right = data["right"];
    state.rotationDown = data["rotation_down"];
    state.rotationUp = data["rotation_up"];
    state.extendsUnextended = data["extends_unextended"];
    state.extendsExtended = data["extends_extended"];
    state.picked = data["picked"];

    std::string mode = data["mode"].get<std::string>();
    if(mode == "automatic")
        state.mode = RoboticHandCore::ModeAutomatic;
    else if(mode == "manual")
        state.mode = RoboticHandCore::ModeManual;
    else
        state.mode = RoboticHandCore::ModeLock;

    if(m_state != state)
    {
        m_state = state;
        updateState();
    }
}
