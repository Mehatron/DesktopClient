#ifndef _W_CENTRAL_
#define _W_CENTRAL_

#include <QWidget>
#include <thread>
#include <mutex>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

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

    void connect(const QString &address);
    void disconnect(void);

private:
    typedef websocketpp::client<websocketpp::config::asio_client> WSClient;

    void setupGUI(void);
    void update(void);
    void messageRecived(const websocketpp::connection_hdl &hdl, const WSClient::message_ptr &msg);

    WRoboticHand *m_wRoboticHand;
    WControl *m_wControl;
    Joypad *m_joypad;

    WSClient m_socket;
    std::thread m_reciverThread;
    std::mutex m_mutexState;
    RoboticHandCore::State m_state;
};

#endif // _W_CENTRAL_
