#include "mainwindow.h"

#include <QtGui>
#include <QtWidgets>

#include "exception.h"
#include "wcentral.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /*
     *  Set global parameters
     */
    QCoreApplication::setApplicationName(tr("Robotic Hand"));
    QCoreApplication::setOrganizationName(tr("Mehatron"));
    QCoreApplication::setOrganizationDomain(tr("https://github.com/Mehatron/"));

    setWindowTitle(QCoreApplication::applicationName());

    setupActions();
    setupMenu();
    setupGUI();
}

MainWindow::~MainWindow(void)
{
}

void MainWindow::setupActions(void)
{
    m_actionAbout = new QAction(tr("&About"), this);
    m_actionAbout->setShortcut(tr("F1"));
    m_actionAbout->setToolTip(tr("About software"));
    m_actionAbout->setStatusTip(tr("About software"));
    connect(m_actionAbout, SIGNAL(triggered(bool)),
            this, SLOT(actionAboutClick()));

    m_actionConnect = new QAction(tr("&Connect"), this);
    m_actionConnect->setShortcut(tr("Ctrl+C"));
    m_actionConnect->setToolTip(tr("Connect to SCADA server"));
    m_actionConnect->setStatusTip(tr("Connect to SCADA server"));
    connect(m_actionConnect, SIGNAL(triggered(bool)),
            this, SLOT(actionConnectClick()));

    m_actionQuit = new QAction(tr("&Quit"), this);
    m_actionQuit->setShortcut(tr("Ctrl+Q"));
    m_actionQuit->setToolTip(tr("Quit software"));
    m_actionQuit->setStatusTip(tr("Quit software"));
    connect(m_actionQuit, SIGNAL(triggered(bool)),
            qApp, SLOT(quit()));

    QDir dir(tr("/dev/input/"));
    dir.setNameFilters(QStringList() << "js*");
    QStringList devices = dir.entryList(QDir::System);
    for(auto device : devices)
    {
        QAction *actionDevice = new QAction(tr("/dev/input/") + device, this);
        connect(actionDevice, SIGNAL(triggered(bool)),
                this, SLOT(actionJoystickClick()));
        m_actionJoystick.append(actionDevice);
    }
}

void MainWindow::setupMenu(void)
{
    QMenu *file = menuBar()->addMenu(tr("&File"));
    file->addAction(m_actionConnect);
    QMenu *joystick = file->addMenu(tr("&Joystick"));
    file->addSeparator();
    file->addAction(m_actionQuit);

    for(auto action : m_actionJoystick)
        joystick->addAction(action);

    QMenu *help = menuBar()->addMenu(tr("&Help"));
    help->addAction(m_actionAbout);
}

void MainWindow::setupGUI(void)
{
    m_centralWidget = new WCentral(this);
    setCentralWidget(m_centralWidget);
}

void MainWindow::actionConnectClick(void)
{
    bool ok;
    QString address = QInputDialog::getText(this, tr("Connect"),
                                            tr("Server address:"),
                                            QLineEdit::Normal, tr("192.168.8.100"),
                                            &ok);
    if(ok)
    {
        address = "ws://" + address + ":8272/";
        try {
            m_centralWidget->connect(address);
        } catch(Exception &ex) {
            QMessageBox::critical(this, tr("Robotic Hand"),
                                  tr("Connection to SCADA server failed"));
        }
    }
}

void MainWindow::actionJoystickClick(void)
{
    QString device = ((QAction *)sender())->text();
    m_centralWidget->openJoystick(device);
}

void MainWindow::actionAboutClick(void)
{
    QString message = tr(
        "<b>%1</b><hr />"
        "Client SCADA application for %1<hr />"
        "(C) 2016-2017 Milos Zivlak, Pavle Kukavica, Milan Prica"
    ).arg(
        QCoreApplication::applicationName()     // %1
    );
    QMessageBox::about(this, tr("Robotic Hand"), message);
}
