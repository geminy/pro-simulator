#ifndef SOCKET_H
#define SOCKET_H

#include <QTcpSocket>
#include <QTimer>

/* @address address:port */
class Socket : public QTcpSocket
{
    Q_OBJECT

public:
    static Socket* Instance();
    static void Destroy();

    QList<QString> getHostips();
    bool beginConnect(const QString &address);
    bool beginDisconnect(const QString &address);
    void sendData(const char *data, int len);

signals:
    void socketStateChanged(const QString &state);

private slots:
    void onStateChanged(QAbstractSocket::SocketState socketState);

private:
    Socket(QTcpSocket *parent = 0);
    Socket(const Socket &socket);
    Socket& operator=(const Socket &socket);
    ~Socket();

private:
    static Socket *s_pSelf;

    QString m_address;
};

#endif // SOCKET_H
