#include "mainwindow/statusbar.h"

StatusBar::StatusBar(QWidget *parent)
    : QStatusBar(parent)
{
}

StatusBar::~StatusBar()
{
}

void StatusBar::showStatus(QString msg, int timeout)
{
    showMessage(msg, timeout);
}
