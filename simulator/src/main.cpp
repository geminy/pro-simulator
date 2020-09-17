#include "mainwindow/mainwindow.h"

#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QElapsedTimer>
#include <QDebug>

#define SPLASHSCREEN_SOURCE ":/screen/opening.png"
#define SPLASHSCREEN_MESSAGE "Simulator"
#define SPLASHSCREEN_DELAY (500) // ms

/*
 * 1. No Application Icon on Linux Desktops.
 * 2. A splash screen shows during application startup on desktop center.
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QPixmap pixmap(SPLASHSCREEN_SOURCE);
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage(SPLASHSCREEN_MESSAGE, Qt::AlignCenter, Qt::yellow);
    QElapsedTimer timer;
    timer.start();
    while (timer.elapsed() < SPLASHSCREEN_DELAY) {
        app.processEvents();
    }

    MainWindow window;
    window.show();
    splash.finish(&window);

    return app.exec();
}
