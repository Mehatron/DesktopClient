#include "wcontrol.h"

#include <QtGui>
#include <QtWidgets>
#include <QGridLayout>

WControl::WControl(QWidget *parent)
    : QWidget(parent)
{
    setupGUI();
}

WControl::~WControl(void)
{
}

void WControl::setupGUI(void)
{
    /*
     * Create widgets
     */

    m_btnMode = new QPushButton(tr("Mode"), this);
    m_btnMode->setShortcut(tr("M"));
    m_btnMode->setToolTip(tr("Operation mode"));
    m_btnMode->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    connect(m_btnMode, &QPushButton::clicked, [this](bool) {
        if(m_btnMode->text() == tr("Automatic"))
            emit sendCommand("mode_automatic");
        else
            emit sendCommand("mode_manual");
    });

    m_btnMoveUp = new QPushButton(tr("^"), this);
    m_btnMoveUp->setShortcut(tr("Up"));
    m_btnMoveUp->setToolTip(tr("Move construction up"));
    connect(m_btnMoveUp, &QPushButton::clicked, [this](bool) {
        emit sendCommand("move_up");
    });

    m_btnMoveDown = new QPushButton(tr("v"), this);
    m_btnMoveDown->setShortcut(tr("Down"));
    m_btnMoveDown->setToolTip(tr("Move construction down"));
    connect(m_btnMoveDown, &QPushButton::clicked, [this](bool) {
        emit sendCommand("move_down");
    });

    m_btnMoveRight = new QPushButton(tr(">"), this);
    m_btnMoveRight->setShortcut(tr("Right"));
    m_btnMoveRight->setToolTip(tr("Move construction right"));
    connect(m_btnMoveRight, &QPushButton::clicked, [this](bool) {
        emit sendCommand("move_right");
    });

    m_btnMoveLeft = new QPushButton(tr("<"), this);
    m_btnMoveLeft->setShortcut(tr("Left"));
    m_btnMoveLeft->setToolTip(tr("Move construction left"));
    connect(m_btnMoveLeft, &QPushButton::clicked, [this](bool) {
        emit sendCommand("move_left");
    });

    m_btnRotateUp = new QPushButton(tr("Rotate Up"), this);
    m_btnRotateUp->setShortcut(tr("D"));
    m_btnRotateUp->setToolTip(tr("Rotate up"));
    connect(m_btnRotateUp, &QPushButton::clicked, [this](bool) {
        emit sendCommand("rotate_up");
    });

    m_btnRotateDown = new QPushButton(tr("Rotate Down"), this);
    m_btnRotateDown->setShortcut(tr("A"));
    m_btnRotateDown->setToolTip(tr("Rotate down"));
    connect(m_btnRotateDown, &QPushButton::clicked, [this](bool) {
        emit sendCommand("rotate_down");
    });

    m_btnExtend = new QPushButton(tr("Extend"), this);
    m_btnExtend->setShortcut(tr("S"));
    m_btnExtend->setToolTip(tr("Extend"));
    connect(m_btnExtend, &QPushButton::clicked, [this](bool) {
        emit sendCommand("extend");
    });

    m_btnUnExtend = new QPushButton(tr("Un Extend"), this);
    m_btnUnExtend->setShortcut(tr("W"));
    m_btnUnExtend->setToolTip(tr("Un Extend"));
    connect(m_btnUnExtend, &QPushButton::clicked, [this](bool) {
        emit sendCommand("unextend");
    });

    m_btnPick = new QPushButton(tr("Pick"), this);
    m_btnPick->setShortcut(tr("E"));
    m_btnPick->setToolTip(tr("Pick"));
    connect(m_btnPick, &QPushButton::clicked, [this](bool) {
        emit sendCommand("pick");
    });

    m_btnPlace = new QPushButton(tr("Place"), this);
    m_btnPlace->setShortcut(tr("Q"));
    m_btnPlace->setToolTip(tr("Place"));
    connect(m_btnPlace, &QPushButton::clicked, [this](bool) {
        emit sendCommand("place");
    });

    /*
     * Laydown widgets
     */

    QGridLayout *mainLayout = new QGridLayout;

    mainLayout->addWidget(m_btnMode, 0, 0, 2, 1);

    mainLayout->addWidget(m_btnMoveUp, 0, 2);
    mainLayout->addWidget(m_btnMoveLeft, 1, 1);
    mainLayout->addWidget(m_btnMoveDown, 1, 2);
    mainLayout->addWidget(m_btnMoveRight, 1, 3);

    mainLayout->addWidget(m_btnRotateUp, 0, 4);
    mainLayout->addWidget(m_btnRotateDown, 1, 4);

    mainLayout->addWidget(m_btnExtend, 0, 5);
    mainLayout->addWidget(m_btnUnExtend, 1, 5);

    mainLayout->addWidget(m_btnPick, 0, 6);
    mainLayout->addWidget(m_btnPlace, 1, 6);

    setLayout(mainLayout);
}

void WControl::setState(const RoboticHandCore::State &state)
{
    if(state.mode == RoboticHandCore::ModeLock)
    {
        if(!m_btnMode->isEnabled())
            return;
        m_btnMode->setEnabled(false);
        m_btnMoveUp->setEnabled(false);
        m_btnMoveDown->setEnabled(false);
        m_btnMoveRight->setEnabled(false);
        m_btnMoveLeft->setEnabled(false);
        m_btnRotateUp->setEnabled(false);
        m_btnRotateDown->setEnabled(false);
        m_btnExtend->setEnabled(false);
        m_btnUnExtend->setEnabled(false);
        m_btnPick->setEnabled(false);
        m_btnPlace->setEnabled(false);
    } else
    {
        if(state.mode == RoboticHandCore::ModeAutomatic)
            m_btnMode->setText(tr("Manual"));
        else if(state.mode == RoboticHandCore::ModeManual)
            m_btnMode->setText(tr("Automatic"));
        m_btnMode->setShortcut(tr("M"));

        if(m_btnMode->isEnabled())
            return;
        m_btnMode->setEnabled(true);
        m_btnMoveUp->setEnabled(true);
        m_btnMoveDown->setEnabled(true);
        m_btnMoveRight->setEnabled(true);
        m_btnMoveLeft->setEnabled(true);
        m_btnRotateUp->setEnabled(true);
        m_btnRotateDown->setEnabled(true);
        m_btnExtend->setEnabled(true);
        m_btnUnExtend->setEnabled(true);
        m_btnPick->setEnabled(true);
        m_btnPlace->setEnabled(true);
    }
}
