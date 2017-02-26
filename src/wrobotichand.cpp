#include  "wrobotichand.h"

#include <QtGui>
#include "json.hpp"

#include "exception.h"

WRoboticHand::WRoboticHand(QWidget *parent)
    : QWidget(parent)
{
    try {
        openConnection("ws://localhost:8272/");
    } catch(Exception &ex) {
        qDebug() << ex;
    }
}

WRoboticHand::~WRoboticHand(void)
{
    try {
        openConnection();
    } catch(Exception &ex) {}
}

void WRoboticHand::openConnection(const QString &url)
{
    try {
        m_socket.set_message_handler([this](const websocketpp::connection_hdl &hdl,
                                            const WSClient::message_ptr &msg)
            {
                messageRecived(hdl, msg);
            });
        m_socket.init_asio();

        websocketpp::lib::error_code error;
        WSClient::connection_ptr con = m_socket.get_connection(url.toStdString(), error);
        if(error)
            throw Exception("Communication error: " + std::string(error.message()));

        m_socket.connect(con);
        m_reciverThread = std::thread([this]()
            {
                m_socket.run();
            });
    } catch(websocketpp::exception &ex) {
        throw Exception("Communication error: " + std::string(ex.what()));
    }
}

void WRoboticHand::openConnection(void)
{
    try {
        if(m_reciverThread.joinable())
			m_reciverThread.join();
    } catch(websocketpp::exception &ex) {
        throw Exception("Communication error: " + std::string(ex.what()));
    }
}

void WRoboticHand::messageRecived(const websocketpp::connection_hdl &hdl, const WSClient::message_ptr &msg)
{
    std::string message = msg->get_payload();
    nlohmann::json data = nlohmann::json::parse(message);

    State state;
    state.constructionDown = data["construction_down"];
    state.constructionUp = data["construction_up"];
    state.left= data["construction_left"];
    state.right = data["construction_right"];
    state.rotationDown = data["rotation_down"];
    state.rotationUp = data["rotation_up"];
    state.extendsUnextended = data["extends_unextended"];
    state.extendsExtended = data["extends_extended"];
    state.picked = data["picked"];

    std::string mode = data["mode"].get<std::string>();
    if(mode == "automatic")
        state.mode = ModeAutomatic;
    else if(mode == "manual")
        state.mode = ModeManual;
    else
        state.mode = ModeLock;

    if(m_state != state)
    {
        m_state = state;
        repaint();
    }
}

void WRoboticHand::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    /*
     * Draw construction
     */
    p.setBrush(Qt::green);
    p.drawRect(10, height() / 2, width() * 2 / 3, 25);

    /*
     * Draw rotation
     */
    p.setBrush(Qt::red);
    p.drawEllipse(50, 50, 10, 10);
}

bool operator==(const WRoboticHand::State &lhs, const WRoboticHand::State &rhs)
{
    if(lhs.mode != rhs.mode ||
        lhs.constructionDown != rhs.constructionDown ||
        lhs.constructionUp != rhs.constructionUp ||
        lhs.left != rhs.left ||
        lhs.right != rhs.right ||
        lhs.rotationDown != rhs.rotationDown ||
        lhs.rotationUp != rhs.rotationUp ||
        lhs.extendsUnextended != rhs.extendsUnextended ||
        lhs.extendsExtended != rhs.extendsExtended ||
        lhs.picked != rhs.picked)
        return false;

    return true;
}
