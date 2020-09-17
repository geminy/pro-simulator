#include "socket/socket.h"
#include "configparser/configinfo.h"
#include "configparser/configparser.h"

#include <QHostAddress>

Socket* Socket::s_pSelf = 0;

Socket* Socket::Instance()
{
    if (0 == s_pSelf) {
        s_pSelf = new Socket;
    }
    return s_pSelf;
}

void Socket::Destroy()
{
    if (0 != s_pSelf) {
        delete s_pSelf;
        s_pSelf = 0;
    }
}

QList<QString> Socket::getHostips()
{
    QList<QString> ipList;
    QList<simulator::SocketInfo> socketInfoList = simulator::GetSocketInfo();
    foreach (simulator::SocketInfo socketInfo, socketInfoList) {
        ipList.append(QString(socketInfo.address).append(":").append(socketInfo.port));
    }
    return ipList;
}

bool Socket::beginConnect(const QString &address)
{
//    return true;

    if (m_address != address) {
        m_address = address;
        abort();
    }

    if (state() == UnconnectedState) {
        QStringList ip = address.split(":");
        Q_ASSERT(2 == ip.length());
        QString addr = ip.at(0);
        QString port = ip.at(1);
        connectToHost(addr, port.toUShort());
    }

    if (state() == ConnectedState) {
        return true;
    }

    return false;
}

bool Socket::beginDisconnect(const QString &address)
{
//    return true;

    Q_ASSERT(m_address == address);
    disconnectFromHost();

    return false;
}

void Socket::sendData(const char *data, int len)
{
    QStringList dataList;
    for (int i = 0; i < len; ++i) {
        dataList.append(QString::number(QString::number(QChar(data[i]).unicode()).toInt(), 16).toUpper());
        qDebug() << dataList.last();
    }
    qDebug() << "--" ;

    write(data, len);
}

void Socket::onStateChanged(QAbstractSocket::SocketState socketState)
{
    QString message = m_address;

    if (UnconnectedState == socketState) {
        message.append(QString(" UnconnectedState."));
    }
    else if (HostLookupState == socketState) {
        message.append(QString(" HostLookupState."));
    }
    else if (ConnectingState == socketState) {
        message.append(QString(" ConnectingState."));
    }
    else if (ConnectedState == socketState) {
        message.append(QString(" ConnectedState."));
    }
    else if (BoundState == socketState) {
        message.append(QString(" BoundState."));
    }
    else if (ClosingState == socketState) {
        message.append(QString(" ClosingState."));
    }
    else if (ListeningState == socketState) {
        message.append(QString(" ListeningState."));
    }
    else {
        message.append(QString(" Unknown socket state!"));
    }

    emit socketStateChanged(message);
}

Socket::Socket(QTcpSocket *parent)
    : QTcpSocket(parent)
    , m_address(QString("Unknown"))
{
    connect(this, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onStateChanged(QAbstractSocket::SocketState)));
}

Socket::~Socket()
{
    disconnect(this, 0, 0, 0);
}
