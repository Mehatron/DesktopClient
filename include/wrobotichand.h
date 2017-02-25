#ifndef _W_ROBOTIC_HAND_H_
#define _W_ROBOTIC_HAND_H_

#include <QWidget>
#include <thread>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

class WRoboticHand : public QWidget
{
public:
    WRoboticHand(QWidget *parent = nullptr);
    ~WRoboticHand(void);

    void openCommunication(const QString &uri);
    void closeCommunication(void);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    typedef websocketpp::client<websocketpp::config::asio_client> WSClient;

    WSClient m_socket;

	std::thread m_reciverThread;
};

#endif // _W_ROBOTIC_HAND_H_
