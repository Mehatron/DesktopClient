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
    : QWidget(parent)
{
    m_joypad = new Joypad(this);
    QObject::connect(m_joypad, &Joypad::sendCommand,
                     this, &WCentral::sendCommand);

    setupGUI();
}

WCentral::~WCentral(void)
{
    try {
        disconnect();
    } catch(...) {};
}

void WCentral::setupGUI(void)
{
    /*
     * Create widgets
     */

    m_wRoboticHand = new WRoboticHand(this);

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
    layoutMain->addStretch(1);
    layoutMain->addLayout(layoutControl);

    setLayout(layoutMain);
}

void WCentral::connect(const QString &address)
{
    try {
        m_socket.set_message_handler([this](const websocketpp::connection_hdl &hdl,
                                            const WSClient::message_ptr &msg)
            {
                try {
                    messageRecived(hdl, msg);
                } catch(...) {};
            });
        m_socket.init_asio();

        websocketpp::lib::error_code error;
        WSClient::connection_ptr con = m_socket.get_connection(address.toStdString(), error);
        if(error)
            throw Exception("Communication error: " + std::string(error.message()));

        m_hdl = con->get_handle();

        m_socket.connect(con);
        m_reciverThread = std::thread([this]()
            {
                m_socket.run();
            });
    } catch(websocketpp::exception &ex) {
        throw Exception("Communication error: " + std::string(ex.what()));
    }
}

void WCentral::disconnect(void)
{
    try {
        if(m_reciverThread.joinable())
			m_reciverThread.join();
    } catch(websocketpp::exception &ex) {
        throw Exception("Communication error: " + std::string(ex.what()));
    }
}

void WCentral::openJoystick(const QString &device)
{
    m_joypad->open(device);
}

void WCentral::update(void)
{
    m_wControl->setState(m_state);
    m_wRoboticHand->setState(m_state);
}

void WCentral::messageRecived(const websocketpp::connection_hdl &hdl, const WSClient::message_ptr &msg)
{
    std::string message = msg->get_payload();
    nlohmann::json data = nlohmann::json::parse(message);

    RoboticHandCore::State state;
    state.constructionDown = data["construction_down"];
    state.constructionUp = data["construction_up"];
    state.left= data["left"];
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
        update();
    }
}

void WCentral::sendCommand(const QString &command)
{
    try {
        m_socket.send(m_hdl, command.toStdString(), websocketpp::frame::opcode::text);
    } catch(...) {};
}
