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
        w += 25;
    p.setBrush(Qt::yellow);
    p.drawRect(-75, h + 15, w, 15);
    p.drawRect(90 - w, h + 15, w, 15);
    p.drawRect(w - 75, h + 5, 10, 35);
    p.drawRect(90 - w, h + 5, 10, 35);
    p.rotate(angle);

    p.translate(-x, -y);
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
