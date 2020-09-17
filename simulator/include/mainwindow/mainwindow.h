#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MenuBar;
class StatusBar;
class HomeWidget;
class BackButton;
class HardkeyWidget;
class TouchWidget;
class InfoWidget;
class ConfigWidget;

/*
 * Ignores memory, but visibiliby.
 * Uses raise and stackUnder.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void initialize();
    void finalize();
    void stackVisibleWidgets(QWidget *widget, bool clear = false);

private slots:
    void onConnectDone();
    void onDisconnectDone();
    void onHardkeyBtnClicked(bool checked = false);
    void onTouchBtnClicked(bool checked = false);
    void onInfoBtnClicked(bool checked = false);
    void onConfigBtnClicked(bool checked = false);
    void onBackBtnClicked(bool checked = false);

private:
    MenuBar *m_pMenuBar;
    StatusBar *m_pStatusBar;
    QWidget *m_pCentralWidget;
    HomeWidget *m_pHomeWidget;
    HardkeyWidget *m_pHardkey;
    TouchWidget *m_pTouch;
    InfoWidget *m_pInfo;
    ConfigWidget *m_pConfig;
    BackButton *m_pBackButton;
    int m_desktopWidth;
    int m_desktopHeight;
    int m_centralWidth;
    int m_centralHeight;
    QList<QWidget*> m_visibleWidgets;
};

#endif // MAINWINDOW_H
