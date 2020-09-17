#include "mainwindow/menubar.h"
#include "configparser/configinfo.h"
#include "socket/socket.h"

MenuBar::MenuBar(QWidget *parent)
    : QMenuBar(parent)
    , m_pBegin(0)
    , m_pSearch(0)
    , m_pConnect(0)
    , m_pDisconnect(0)
    , m_address("Unknown")
{
    addMenuAndAction();
}

MenuBar::~MenuBar()
{
    disconnect(m_pSearch, SIGNAL(triggered(bool)), this, SLOT(onSearchClicked(bool)));
    disconnect(m_pConnect, SIGNAL(triggered(QAction*)), this, SLOT(onAddressSelected(QAction*)));
    disconnect(m_pDisconnect, SIGNAL(triggered(bool)), this, SLOT(onDisconnectClicked(bool)));
}

void MenuBar::addMenuAndAction()
{
    m_pBegin = addMenu(MENUBAR_BEGIN);
    Q_ASSERT(m_pBegin);
    m_pSearch = m_pBegin->addAction(MENUBAR_SEARCH);
    Q_ASSERT(m_pSearch);
    m_pSearch->setStatusTip(QString("Search available addresses."));
    m_pConnect = addMenu(MENUBAR_CONNECT);
    Q_ASSERT(m_pConnect);
    m_pConnect->setEnabled(false);
    m_pDisconnect = addAction(MENUBAR_DISCONNECT);
    Q_ASSERT(m_pDisconnect);
    m_pDisconnect->setEnabled(false);
    m_pDisconnect->setStatusTip(QString("Disconnect from server."));

    connect(m_pSearch, SIGNAL(triggered(bool)), this, SLOT(onSearchClicked(bool)));
    connect(m_pConnect, SIGNAL(triggered(QAction*)), this, SLOT(onAddressSelected(QAction*)));
    connect(m_pDisconnect, SIGNAL(triggered(bool)), this, SLOT(onDisconnectClicked(bool)));
    connect(Socket::Instance(), SIGNAL(connected()), this, SLOT(onConnected()));
    connect(Socket::Instance(), SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(Socket::Instance(), SIGNAL(socketStateChanged(QString)), this, SLOT(onSocketStateChanged(QString)));
}

void MenuBar::prepareConnect(QList<QString> ipList)
{
    m_pConnect->clear();

    foreach (QString ip, ipList) {
        QAction *action = m_pConnect->addAction(ip);
        action->setStatusTip(ip);
    }

    m_pConnect->setEnabled(true);
}

void MenuBar::onSearchClicked(bool checked)
{
    Q_UNUSED(checked);

    m_pConnect->setEnabled(false);
    m_pDisconnect->setEnabled(false);

    QList<QString> ipList = Socket::Instance()->getHostips();
    if (ipList.length() == 0) {
        emit stateChanged("Sorry, no address available to connect!");
    }
    else {
        emit stateChanged("Connect to server.");
        prepareConnect(ipList);
    }

    /* fake disconnect, disable CentralWidget of MainWindow */
    emit disconnectDone();
}

void MenuBar::onAddressSelected(QAction *action)
{
    m_address = action->text();
    emit stateChanged(m_address + QString(" connecting..."));

    bool result = Socket::Instance()->beginConnect(m_address);
    if (result) {
        emit stateChanged(m_address + QString(" connect succeeded imediately."));
        m_pConnect->setEnabled(false);
        m_pDisconnect->setEnabled(true);
        emit connectDone();
    }
}

void MenuBar::onDisconnectClicked(bool checked)
{
    Q_UNUSED(checked);

    emit stateChanged(m_address + QString(" disconnecting..."));
    bool result = Socket::Instance()->beginDisconnect(m_address);

    if (result) {
        emit stateChanged(m_address + QString(" disconnect succeeded imediately."));
        m_pDisconnect->setEnabled(false);
        emit disconnectDone();
    }
}

void MenuBar::onConnected()
{
    emit stateChanged(m_address + QString(" connect succeeded."));

    m_pConnect->setEnabled(false);
    m_pDisconnect->setEnabled(true);
    emit connectDone();
}

void MenuBar::onDisconnected()
{
    emit stateChanged(m_address + QString(" disconnect succeeded."));

    m_pDisconnect->setEnabled(false);
    emit disconnectDone();
}

void MenuBar::onSocketStateChanged(const QString &state)
{
    emit stateChanged(state);
}
