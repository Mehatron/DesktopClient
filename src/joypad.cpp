#include "joypad.h"

#include <chrono>

#include "exception.h"

Joypad::Joypad(QObject *parent)
    : QObject(parent),
      m_threadRunning(false)
{
}

Joypad::Joypad(int joystick, QObject *parent)
    : QObject(parent),
      m_joystick(joystick),
      m_threadRunning(false)
{
    open(joystick);
}

Joypad::~Joypad(void)
{
    close();
}

void Joypad::open(int joystick)
{
    m_joystick = Joystick(joystick);
    if(!m_joystick.isFound())
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
            if(!m_joystick.sample(&event))
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
                            emit sendCommand("motor3_stop_right");
                            break;
                        case 7:
                            emit sendCommand("motor3_stop_left");
                            break;
                    }
                }
            } else if(event.isAxis())
            {
                switch(event.number)
                {
                    case 0:
                        if(event.value > Joypad::AnalogIntensity)
                            emit sendCommand("move_right");
                        else if(event.value < -Joypad::AnalogIntensity)
                            emit sendCommand("move_left");
                        break;
                    case 1:
                        if(event.value > Joypad::AnalogIntensity)
                            emit sendCommand("move_down");
                        else if(event.value < -Joypad::AnalogIntensity)
                            emit sendCommand("move_up");
                }
            }
        }
    }
}
