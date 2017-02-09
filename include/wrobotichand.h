#ifndef _W_ROBOTIC_HAND_H_
#define _W_ROBOTIC_HAND_H_

#include <QWidget>

class WRoboticHand : public QWidget
{
public:
    WRoboticHand(void);
    ~WRoboticHand(void);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
};

#endif // _W_ROBOTIC_HAND_H_
