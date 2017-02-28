#ifndef _MAIN_WINDOW_
#define _MAIN_WINDOW_

#include <QMainWindow>

class QAction;

class WCentral;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow(void);

private:
    void setupActions(void);
    void setupMenu(void);
    void setupGUI(void);

    WCentral *m_centralWidget;

    QAction *m_actionConnect;
    QList<QAction *> m_actionJoystick;
    QAction *m_actionQuit;

    QAction *m_actionAbout;

private slots:
    void actionConnectClick(void);
    void actionJoystickClick(void);
    void actionAboutClick(void);
};

#endif // _MAIN_WINDOW_
