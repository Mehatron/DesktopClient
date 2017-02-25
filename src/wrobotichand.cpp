#include  "wrobotichand.h"

#include <QtGui>

#include "exception.h"

WRoboticHand::WRoboticHand(QWidget *parent)
    : QWidget(parent)
{
    try {
        openCommunication("ws://localhost:8272/");
    } catch(Exception &ex) {
        qDebug() << ex;
    }
}

WRoboticHand::~WRoboticHand(void)
{
    try {
        closeCommunication();
    } catch(Exception &ex) {}
}

void WRoboticHand::openCommunication(const QString &uri)
{
    try {
		m_socket.set_message_handler([this](const websocketpp::connection_hdl &hdl,
											const WSClient::message_ptr &msg)
			{
				qDebug() << "Radi!";
			});
        m_socket.init_asio();

        websocketpp::lib::error_code error;
        WSClient::connection_ptr con = m_socket.get_connection(uri.toStdString(), error);
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

void WRoboticHand::closeCommunication(void)
{
    try {
		if(m_reciverThread.joinable())
			m_reciverThread.join();
    } catch(websocketpp::exception &ex) {
        throw Exception("Communication error: " + std::string(ex.what()));
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
}
