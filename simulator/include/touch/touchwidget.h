#ifndef TOUCHWIDGET_H
#define TOUCHWIDGET_H

#include <QWidget>
#include <QCursor>
#include <QPalette>

namespace Ui
{
    class Touch;
}

/* panel widget */
class TouchWidget : public QWidget
{
    Q_OBJECT

public:
    enum TOUCHWIDGET_TYPE
    {
        TOUCHWIDGET_TOUCH,
        TOUCHWIDGET_PANEL,
        TOUCHWIDGET_PANEL_NARROW,
        TOUCHWIDGET_PANEL_WIDE,
        TOUCHWIDGET_PAD,
        TOUCHWIDGET_PAD_RELATIVE,
        TOUCHWIDGET_PAD_ABSOLUTE,
        TOUCHWIDGET_NONE
    };

    enum TOUCHWIDGET_MOUSEEVENT_TYPE
    {
        TOUCHWIDGET_MOUSEEVENT_PRESS,
        TOUCHWIDGET_MOUSEEVENT_RELEASE,
        TOUCHWIDGET_MOUSEEVENT_MOVE,
        TOUCHWIDGET_MOUSEEVENT_DOUBLECLICK,
        TOUCHWIDGET_MOUSEEVENT_NONE
    };

    enum TOUCHPANELDATA
    {
        PANELDATA_BYTE_HEAD_FIXED = 15,
        PANELDATA_BYTE_HX1 = 1,
        PANELDATA_BYTE_LX1 = 1,
        PANELDATA_BYTE_HY1 = 1,
        PANELDATA_BYTE_LY1 = 1,
        PANELDATA_BYTE_HX2 = 1,
        PANELDATA_BYTE_LX2 = 1,
        PANELDATA_BYTE_HY2 = 1,
        PANELDATA_BYTE_LY2 = 1,
        PANELDATA_BYTE_COUNT = 1,
        PANELDATA_BYTE_CHECKSUM = 1,
        PANELDATA_BYTE_TAIL_FIXED = 2,
        PANELDATA_BYTE_TOTALSUM = 27,

        PANELDATA_INDEX_HX1 = 15,
        PANELDATA_INDEX_LX1 = 16,
        PANELDATA_INDEX_HY1 = 17,
        PANELDATA_INDEX_LY1 = 18,
        PANELDATA_INDEX_HX2 = 19,
        PANELDATA_INDEX_LX2 = 20,
        PANELDATA_INDEX_HY2 = 21,
        PANELDATA_INDEX_LY2 = 22,
        PANELDATA_INDEX_COUNT = 23,
        PANELDATA_INDEX_CHECKSUM = 24,
        PANELDATA_INDEX_CHECKSUM_BEGIN = 2,
        PANELDATA_INDEX_CHECKSUM_END = 23,
    };

    static TouchWidget* Instance();

    explicit TouchWidget(QWidget *parent);
    ~TouchWidget();
    void showTouchState(const QString &state, bool show = false, const QPoint &pos = QPoint());

private:
    void setEndianState();
    void initPanelData();
    void parseMouseEvent(QMouseEvent *event);
    void parsePanelData(const QPoint &pos);
    void sendData();

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

signals:
    void padTypeChanged(const QString &type);

private slots:
    void on_touchTab_currentChanged(int index);
    void on_panelResolution_currentChanged(int index);
    void on_padCoord_currentChanged(int index);

private:
    static TouchWidget* s_pSelf;

    Ui::Touch *m_pTouch;
    TOUCHWIDGET_TYPE m_touchType;
    TOUCHWIDGET_TYPE m_panelType;
    TOUCHWIDGET_TYPE m_padType;
    TOUCHWIDGET_MOUSEEVENT_TYPE m_mouseEventType;
    bool m_isBigEndian;
    char m_panelData[PANELDATA_BYTE_TOTALSUM + 1];
};

#endif // TOUCHWIDGET_H
