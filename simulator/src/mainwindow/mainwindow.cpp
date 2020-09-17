#include "mainwindow/mainwindow.h"
#include "mainwindow/menubar.h"
#include "mainwindow/statusbar.h"
#include "mainwindow/homewidget.h"
#include "hardkey/hardkeywidget.h"
#include "touch/touchwidget.h"
#include "info/infowidget.h"
#include "config/configwidget.h"
#include "mainwindow/backbutton.h"
#include "configparser/configinfo.h"
#include "socket/socket.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QIcon>
#include <QMenuBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_pMenuBar(new MenuBar)
    , m_pStatusBar(new StatusBar)
    , m_pCentralWidget(new QWidget)
    , m_pHomeWidget(0)
    , m_pHardkey(0)
    , m_pTouch(0)
    , m_pInfo(0)
    , m_pConfig(0)
    , m_pBackButton(0)
    , m_desktopWidth(0)
    , m_desktopHeight(0)
    , m_centralWidth(0)
    , m_centralHeight(0)
{
    initialize();

    connect(m_pMenuBar, SIGNAL(stateChanged(QString, int)), m_pStatusBar, SLOT(showStatus(QString, int)));
    connect(m_pMenuBar, SIGNAL(connectDone()), this, SLOT(onConnectDone()));
    connect(m_pMenuBar, SIGNAL(disconnectDone()), this, SLOT(onDisconnectDone()));

    connect(m_pHomeWidget, SIGNAL(hardkeyBtnClicked(bool)), this, SLOT(onHardkeyBtnClicked(bool)));
    connect(m_pHomeWidget, SIGNAL(touchBtnClicked(bool)), this, SLOT(onTouchBtnClicked(bool)));
    connect(m_pHomeWidget, SIGNAL(infoBtnClicked(bool)), this, SLOT(onInfoBtnClicked(bool)));
    connect(m_pHomeWidget, SIGNAL(configBtnClicked(bool)), this, SLOT(onConfigBtnClicked(bool)));

    connect(m_pBackButton, SIGNAL(clicked(bool)), this, SLOT(onBackBtnClicked(bool)));
}

/* QObjectPrivate::deleteChildren */
MainWindow::~MainWindow()
{
    disconnect(m_pMenuBar, 0, 0, 0);
    disconnect(m_pHomeWidget, 0, 0, 0);
    disconnect(m_pBackButton, 0, 0, 0);

    finalize();
}

void MainWindow::initialize()
{
    /* desktop widget */
    QDesktopWidget *desktop = QApplication::desktop();
    Q_ASSERT(desktop);
    Q_ASSERT(desktop->screen());
    m_desktopWidth = desktop->screen()->width();
    m_desktopHeight = desktop->screen()->height();

    /* geometry: desktop center,  fixed size */
    int x = (m_desktopWidth - WINDOW_SIZE_WIDTH) / 2;
    int y = (m_desktopHeight - WINDOW_SIZE_HEIGHT) / 2;
    setGeometry(x, y, 0, 0);
    setFixedSize(WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT);

    /* title */
    setWindowTitle(WINDOW_TITLE);

    /* icon: no effect on Linux */
    setWindowIcon(QIcon(WINDOW_ICON));

    /* removes WindowMaximizeButton, related to window size */
    setWindowFlags(windowFlags() & (~Qt::WindowMaximizeButtonHint));

    /* menu bar */
    setMenuBar(m_pMenuBar);

    /* status bar */
    setStatusBar(m_pStatusBar);
    m_pStatusBar->showStatus("Please click 'Begin' or 'Alt + B' to start.");

    /* central widget, interchange width and height */
    setCentralWidget(m_pCentralWidget);
    m_centralWidth = m_pCentralWidget->width();
    m_centralHeight = m_pCentralWidget->height();

    /* creates all widgets */
    m_pHomeWidget = new HomeWidget(m_pCentralWidget);
    m_pHardkey = new HardkeyWidget(m_pCentralWidget);
    m_pTouch = new TouchWidget(m_pCentralWidget);
    m_pInfo = new InfoWidget(m_pCentralWidget);
    m_pConfig = new ConfigWidget(m_pCentralWidget);
    m_pBackButton = new BackButton(m_pCentralWidget);
    int bbw = m_pBackButton->width();
    int bbh = m_pBackButton->height();
    int bbx = m_centralHeight - bbw - 10; // x-height space(width+10)
    int bby = m_centralWidth - bbh - 10 - 50; // y-width space(height+10+others)
    m_pBackButton->setGeometry(bbx, bby, bbw, bbh);

    /* shows home widget and disable central widget */
    m_pHomeWidget->show();
    stackVisibleWidgets(m_pHomeWidget, true);
    m_pHomeWidget->raise();
    m_pCentralWidget->setEnabled(false);
}

void MainWindow::finalize()
{
    Socket::Destroy();
}

void MainWindow::stackVisibleWidgets(QWidget *widget, bool clear)
{
    if (clear) {
        foreach (QWidget *w, m_visibleWidgets) {
            w->hide();
        }
        m_visibleWidgets.clear();
    }

    m_visibleWidgets.append(widget);
}

void MainWindow::onConnectDone()
{
    m_pCentralWidget->setEnabled(true);
}

void MainWindow::onDisconnectDone()
{
    m_pCentralWidget->setEnabled(false);
}

void MainWindow::onHardkeyBtnClicked(bool checked)
{
    Q_UNUSED(checked);

    m_pHardkey->show();
    stackVisibleWidgets(m_pHardkey, true);
    m_pBackButton->show();
    stackVisibleWidgets(m_pBackButton);
    m_pBackButton->raise();
    m_pHardkey->stackUnder(m_pBackButton);
}

void MainWindow::onTouchBtnClicked(bool checked)
{
    Q_UNUSED(checked);

    m_pTouch->show();
    stackVisibleWidgets(m_pTouch, true);
    m_pBackButton->show();
    stackVisibleWidgets(m_pBackButton);
    m_pBackButton->raise();
    m_pTouch->stackUnder(m_pBackButton);
}

void MainWindow::onInfoBtnClicked(bool checked)
{
    Q_UNUSED(checked);

    m_pInfo->show();
    stackVisibleWidgets(m_pInfo, true);
    m_pBackButton->show();
    stackVisibleWidgets(m_pBackButton);
    m_pBackButton->raise();
    m_pInfo->stackUnder(m_pBackButton);
}

void MainWindow::onConfigBtnClicked(bool checked)
{
    Q_UNUSED(checked);

    m_pConfig->show();
    stackVisibleWidgets(m_pConfig, true);
    m_pBackButton->show();
    stackVisibleWidgets(m_pBackButton);
    m_pBackButton->raise();
    m_pConfig->stackUnder(m_pBackButton);
}

void MainWindow::onBackBtnClicked(bool checked)
{
    Q_UNUSED(checked);

    m_pHomeWidget->show();
    stackVisibleWidgets(m_pHomeWidget, true);
    m_pHomeWidget->raise();
}
