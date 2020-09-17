#ifndef MENUBAR_H
#define MENUBAR_H

#include <QMenuBar>

class MenuBar : public QMenuBar
{
    Q_OBJECT

public:
    explicit MenuBar(QWidget *parent = 0);
    ~MenuBar();

private:
    void addMenuAndAction();
    void prepareConnect(QList<QString> ipList);

signals:
    void stateChanged(const QString &msg, int timeout = 0);
    void connectDone();
    void disconnectDone();

private slots:
    void onSearchClicked(bool checked);
    void onAddressSelected(QAction *action);
    void onDisconnectClicked(bool checked);
    void onConnected();
    void onDisconnected();
    void onSocketStateChanged(const QString &state);

private:
    QMenu *m_pBegin;
    QAction *m_pSearch;
    QMenu *m_pConnect;
    QAction *m_pDisconnect;
    QString m_address;
};

#endif // MENUBAR_H
