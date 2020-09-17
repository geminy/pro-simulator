#include "mainwindow/backbutton.h"
#include "configparser/configinfo.h"

BackButton::BackButton(QWidget *parent)
    : QPushButton(parent)
{
    setVisible(false);
    setText(BUTTON_BACK);
    setFixedSize(BUTTON_BACK_WIDTH, BUTTON_BACK_HEIGHT);
    setFlat(true); // no frame
}

BackButton::~BackButton()
{
}
