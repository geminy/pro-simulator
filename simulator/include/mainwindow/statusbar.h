#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QStatusBar>

class StatusBar : public QStatusBar
{
    Q_OBJECT

public:
    explicit StatusBar(QWidget *parent = 0);
    ~StatusBar();

public slots:
    void showStatus(QString msg, int timeout = 0);
};

#endif // STATUSBAR_H
