#ifndef HARDKEYWIDGET_H
#define HARDKEYWIDGET_H

#include <QWidget>
#include <QPushButton>

namespace Ui
{
    class Hardkey;
}

class HardkeyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HardkeyWidget(QWidget *parent);
    ~HardkeyWidget();

private:
    void initConfigData();

private:
    Ui::Hardkey *m_pHardkey;
};


/*
 * Custom Button
 * x
 * y
 * width
 * height
 * text
 * byte
 * ...
 */
class HardkeyButton : public QPushButton
{
    Q_OBJECT

public:
    explicit HardkeyButton(QWidget *parent = 0);
    ~HardkeyButton();

    void setCommonData(const QString &name, int byte, int width, int height);
    void setPosition(int x, int y);

private slots:
    void onClicked(bool checked = false);

private:
    int m_byte;
};

#endif // HARDKEYWIDGET_H
