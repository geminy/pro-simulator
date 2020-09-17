#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>

class QPushButton;

class HomeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HomeWidget(QWidget *parent);
    ~HomeWidget();

signals:
    void hardkeyBtnClicked(bool checked = false);
    void touchBtnClicked(bool checked = false);
    void infoBtnClicked(bool checked = false);
    void configBtnClicked(bool checked = false);

private:
    QPushButton *m_pHardkey;
    QPushButton *m_pTouch;
    QPushButton *m_pInfo;
    QPushButton *m_pConfig;
};

#endif // HOMEWIDGET_H
