#ifndef _W_ROBOTIC_HAND_H_
#define _W_ROBOTIC_HAND_H_

#include <QWidget>
#include <thread>
#include <mutex>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

class WRoboticHand : public QWidget
{
public:
    WRoboticHand(QWidget *parent = nullptr);
    ~WRoboticHand(void);

    enum Mode {
        ModeAutomatic = 0,
        ModeManual,
        ModeLock
    };

    struct State
    {
        Mode mode                   = ModeAutomatic;
        bool constructionDown       = false;
        bool constructionUp         = false;
        bool left                   = false;
        bool right                  = false;
        bool rotationDown           = false;
        bool rotationUp             = false;
        bool extendsUnextended      = false;
        bool extendsExtended        = false;
        bool picked                 = false;
    };

    void openConnection(const QString &url);
    void openConnection(void);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    typedef websocketpp::client<websocketpp::config::asio_client> WSClient;

    void messageRecived(const websocketpp::connection_hdl &hdl, const WSClient::message_ptr &msg);

    WSClient m_socket;
    std::thread m_reciverThread;

    std::mutex m_mutexState;
    State m_state;
};

bool operator==(const WRoboticHand::State &lhs, const WRoboticHand::State &rhs);
inline bool operator!=(const WRoboticHand::State &lhs, const WRoboticHand::State &rhs)
    { return !operator==(lhs, rhs); };

#endif // _W_ROBOTIC_HAND_H_
