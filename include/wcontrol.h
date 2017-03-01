#ifndef _W_CONTROL_H_
#define _W_CONTROL_H_

#include <QWidget>

#include "robotichandcore.h"

class QPushButton;

class WControl : public QWidget
{
    Q_OBJECT

public:
    explicit WControl(QWidget *parent = 0);
    virtual ~WControl(void);

private:
    void setupGUI(void);

    QPushButton *m_btnMode;
    QPushButton *m_btnMoveUp;
    QPushButton *m_btnMoveDown;
    QPushButton *m_btnMoveRight;
    QPushButton *m_btnMoveLeft;
    QPushButton *m_btnRotateUp;
    QPushButton *m_btnRotateDown;
    QPushButton *m_btnExtend;
    QPushButton *m_btnUnExtend;
    QPushButton *m_btnPick;
    QPushButton *m_btnPlace;

public slots:
    void setState(const RoboticHandCore::State &state);

signals:
    void sendCommand(const QString &command);
};

#endif // _W_CONTROL_H_
