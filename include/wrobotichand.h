#ifndef _W_ROBOTIC_HAND_H_
#define _W_ROBOTIC_HAND_H_

#include <QWidget>
#include "robotichandcore.h"

class WRoboticHand : public QWidget
{
    Q_OBJECT

public:
    explicit WRoboticHand(QWidget *parent = nullptr);
    virtual ~WRoboticHand(void);

    void setState(const RoboticHandCore::State &state);

    virtual inline QSize sizeHint(void) const override { return QSize(400, 400); };

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawConstruction(QPainter &p, int x, int y, int w);
    void drawRotation(QPainter &p, int x, int y);
    void drawGrab(QPainter &p, int x, int y);

    RoboticHandCore::State m_state;
};

#endif // _W_ROBOTIC_HAND_H_
