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
                            emit action(ActionRightLeft);
                            break;
                        case 1:
                            emit action(ActionRightDown);
                            break;
                        case 2:
                            emit action(ActionRightRight);
                            break;
                        case 3:
                            emit action(ActionGrab);
                            break;
                        case 4:
                            emit action(ActionMotor1);
                            break;
                        case 6:
                            emit action(ActionMotor2);
                            break;
                        case 5:
                            emit action(ActionMotor3R);
                            break;
                        case 7:
                            emit action(ActionMotor3L);
                            break;
                    }
                } else
                {
                }
            } else if(event.isAxis())
            {
                switch(event.number)
                {
                    case 0:
                        if(event.value > Joypad::AnalogIntensity)
                            emit action(ActionConstructionRight);
                        else if(event.value < -Joypad::AnalogIntensity)
                            emit action(ActionConstructionLeft);
                        break;
                    case 1:
                        if(event.value > Joypad::AnalogIntensity)
                            emit action(ActionConstructionDown);
                        else if(event.value < -Joypad::AnalogIntensity)
                            emit action(ActionConstructionUp);
                }
            }
        }
    }
}
