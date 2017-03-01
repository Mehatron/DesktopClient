#ifndef _ROBOTIC_HAND_CORE_H_
#define _ROBOTIC_HAND_CORE_H_

class RoboticHandCore
{
public:
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

private:
    RoboticHandCore(void) {};
};

bool operator==(const RoboticHandCore::State &lhs, const RoboticHandCore::State &rhs);
inline bool operator!=(const RoboticHandCore::State &lhs, const RoboticHandCore::State &rhs)
    { return !operator==(lhs, rhs); };

#endif // _ROBOTIC_HAND_CORE_H_
