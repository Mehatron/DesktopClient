#ifndef _JOYPAD_H_
#define _JOYPAD_H_

#include <QObject>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include "joystick.hh"

#include "robotichandcore.h"

class Joypad : public QObject
{
    Q_OBJECT

public:
    explicit Joypad(QObject *prent = 0);
    explicit Joypad(const QString &joystick, QObject *parent = 0);
    virtual ~Joypad(void);

    enum AnalogState
    {
        AnalogStateLUp          = 0x01,
        AnalogStateLDown        = 0x02,
        AnalogStateLRight       = 0x04,
        AnalogStateLLeft        = 0x08,
        AnalogStateRUp          = 0x10,
        AnalogStateRDown        = 0x20,
        AnalogStateRRight       = 0x40,
        AnalogStateRLeft        = 0x80
    };

    static const int AnalogIntensity        = 20000;

    void open(const QString &joystick);
    void close(void);

private:
    void update(void);

    std::shared_ptr<Joystick>(m_joystick);

    std::thread m_thread;
    std::mutex m_mutex;
    bool m_threadRunning;
    std::condition_variable m_cvRunning;

    int m_state;

signals:
    void sendCommand(const QString &command);
};

#endif // _JOYPAD_H_
