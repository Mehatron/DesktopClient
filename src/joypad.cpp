#include "joypad.h"

#include <chrono>

#include <QDebug>

#include "exception.h"

Joypad::Joypad(QObject *parent)
    : QObject(parent),
      m_threadRunning(false),
      m_state(0)
{
}

Joypad::Joypad(const QString &joystick, QObject *parent)
    : QObject(parent),
      m_threadRunning(false)
{
    open(joystick);
}

Joypad::~Joypad(void)
{
    close();
}

void Joypad::open(const QString &joystick)
{
    if(m_threadRunning)
        close();

    m_joystick = std::shared_ptr<Joystick>(new Joystick(joystick.toStdString()));
    if(!m_joystick->isFound())
        throw Exception("Can't open joystick");

    std::lock_guard<std::mutex> lkMutex(m_mutex);
    m_threadRunning = true;
    m_thread = std::thread([this]() {
            update();
        });
}

void Joypad::close(void)
{
    std::lock_guard<std::mutex> lkMutex(m_mutex);
    if(!m_threadRunning)
        return;

    m_threadRunning = false;
    m_cvRunning.notify_all();

    try {
        if(m_thread.joinable())
            m_thread.join();
    } catch(...) {};
}

void Joypad::update(void)
{
    std::unique_lock<std::mutex> lkMutex(m_mutex);
    while(m_threadRunning)
    {
        while(m_cvRunning.wait_for(lkMutex, std::chrono::milliseconds(10))
            == std::cv_status::timeout)
        {
            JoystickEvent event;
            if(!m_joystick->sample(&event))
                continue;

            if(event.isButton())
            {
                if(event.value)
                {
                    switch(event.number)
                    {
                        case 0:
                            emit sendCommand("right_left");
                            break;
                        case 1:
                            emit sendCommand("right_down");
                            break;
                        case 2:
                            emit sendCommand("right_right");
                            break;
                        case 3:
                            emit sendCommand("grab_toggle");
                            break;
                        case 4:
                            emit sendCommand("motor1_start");
                            break;
                        case 6:
                            emit sendCommand("motor2_start");
                            break;
                        case 5:
                            emit sendCommand("motor3_start_right");
                            break;
                        case 7:
                            emit sendCommand("motor3_start_left");
                            break;
                    }
                } else
                {
                    switch(event.number)
                    {
                        case 4:
                            emit sendCommand("motor1_stop");
                            break;
                        case 6:
                            emit sendCommand("motor2_stop");
                            break;
                        case 5:
                        case 7:
                            emit sendCommand("motor3_stop");
                            break;
                    }
                }
            } else if(event.isAxis())
            {
                switch(event.number)
                {
                    case 0:
                        if(event.value > Joypad::AnalogIntensity)
                        {
                            if(!(m_state & AnalogStateLRight))
                            {
                                emit sendCommand("move_right");
                                m_state |= AnalogStateLRight;
                            }
                        } else
                        {
                            m_state &= ~AnalogStateLRight;
                        }
                        if(event.value < -Joypad::AnalogIntensity)
                        {
                            if(!(m_state & AnalogStateLLeft))
                            {
                                emit sendCommand("move_left");
                                m_state |= AnalogStateLLeft;
                            }
                        } else
                        {
                            m_state &= ~AnalogStateLLeft;
                        }
                        break;
                    case 1:
                        if(event.value > Joypad::AnalogIntensity)
                        {
                            if(!(m_state & AnalogStateLDown))
                            {
                                emit sendCommand("move_down");
                                m_state |= AnalogStateLDown;
                            }
                        } else
                        {
                            m_state &= ~AnalogStateLDown;
                        }
                        if(event.value < -Joypad::AnalogIntensity)
                        {
                            if(!(m_state & AnalogStateLUp))
                            {
                                emit sendCommand("move_up");
                                m_state |= AnalogStateLUp;
                            }
                        } else
                        {
                            m_state &= ~AnalogStateLUp;
                        }
                        break;
                    case 2:
                        if(event.value > Joypad::AnalogIntensity)
                        {
                            if(!(m_state & AnalogStateRRight))
                            {
                                emit sendCommand("right_right");
                                m_state |= AnalogStateRRight;
                            }
                        } else
                        {
                            m_state &= ~AnalogStateRRight;
                        }
                        if(event.value < -Joypad::AnalogIntensity &&
                            !(m_state & AnalogStateRLeft))
                        {
                            if(!(m_state & AnalogStateRLeft))
                            {
                                emit sendCommand("right_left");
                                m_state |= AnalogStateRLeft;
                            }
                        } else
                        {
                            m_state &= ~AnalogStateRLeft;
                        }
                        break;
                    case 3:
                        if(event.value > Joypad::AnalogIntensity)
                        {
                            if(!(m_state & AnalogStateRDown))
                            {
                                emit sendCommand("right_down");
                                m_state |= AnalogStateRDown;
                            }
                        } else
                        {
                            m_state &= ~(AnalogStateRDown);
                        }
                        if(event.value < -Joypad::AnalogIntensity)
                        {
                            if(!(m_state & AnalogStateRUp))
                            {
                                emit sendCommand("right_up");
                                m_state |= AnalogStateRUp;
                            }
                        } else
                        {
                            m_state &= ~(AnalogStateRUp);
                        }
                        break;
                }
            }
        }
    }
}
