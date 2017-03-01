#ifndef _JOYPAD_H_
#define _JOYPAD_H_

#include <QObject>

#include <thread>
#include <mutex>
#include <condition_variable>
#include "joystick.hh"

#include "robotichandcore.h"

class Joypad : public QObject
{
    Q_OBJECT

public:
    explicit Joypad(QObject *prent = 0);
    explicit Joypad(int joystick, QObject *parent = 0);
    virtual ~Joypad(void);

    static const int AnalogIntensity        = 20000;

    void open(int joystick = 0);
    void close(void);

private:
    void update(void);

    Joystick m_joystick;

    std::thread m_thread;
    std::mutex m_mutex;
    bool m_threadRunning;
    std::condition_variable m_cvRunning;

signals:
    void sendCommand(const QString &command);
};

#endif // _JOYPAD_H_
