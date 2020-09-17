#ifndef TOUCHPADWIDGET_H
#define TOUCHPADWIDGET_H

#include <QWidget>
#include <QCursor>
#include <QPalette>
#include <QTimer>

class TouchPadWidget : public QWidget
{
    Q_OBJECT

public:
    enum TOUCHPAD_TYPE
    {
        PAD_RELATIVE,
        PAD_ABSOLUTE,
        PAD_INVALID
    };

    enum MOUSEEVENT_TYPE
    {
        MOUSE_PRESS,
        MOUSE_RELEASE,
        MOUSE_MOVE,
        MOUSE_DOUBLECLICK,
        MOUSE_UNKNOWN
    };

    enum MOUSEKEY_TYPE
    {
        MOUSE_LEFT,
        MOUSE_RIGHT,
        MOUSE_MIDDLE,
        MOUSE_NONE
    };

    /*
     * touch: left
     * press: right
     * p1
    */
    enum PADDATA_COORD
    {
        COORD_TOUCH = 0X00,
        COORD_DOUBLECLICK = 0X02,
        COORD_PRESS = 0X04,
        COORD_CLICK = 0X06,

        COORD_PRESS_P3= 0X08,
        COORD_PRESS_P2 = 0X10,
        COORD_PRESS_P1 = 0X20,

        COORD_ABSOLUTE = 0X40,
        COORD_RELATIVE = 0X80,
    };

    /* unused */
    enum PADDATA_MOTION
    {
        MOTION_NONE = 0X00,
        MOTION_SHRINK = 0X01,
        MOTION_INVALID = 0X02
    };

    /* unused */
    enum PADDATA_DISTANCE
    {
        DISTANCE_MIN = 0X00,
        DISTANCE_MAX = 0XFF
    };

    enum PADDATA_OFFSET_X
    {
        OFFSET_X_NONE = 0X00,
        OFFSET_X_RIGHT = 0X01,
        OFFSET_X_LEFT = 0X02
    };

    enum PADDATA_OFFSET_Y
    {
        OFFSET_Y_NONE = 0X00,
        OFFSET_Y_DOWN = 0X01,
        OFFSET_Y_UP = 0X02
    };

    enum TOUCHPADDATA
    {
        PADDATA_BYTE_HEAD_FIXED = 15,
        PADDATA_BYTE_BODY = 9,
        PADDATA_BYTE_TAIL_FIXED = 2,
        PADDATA_BYTE_TOTALSUM = 26,

        PADDATA_INDEX_COORD = 15,
        PADDATA_INDEX_MOTION = 16,
        PADDATA_INDEX_DISTANCE = 17,
        PADDATA_INDEX_VX = 18,
        PADDATA_INDEX_VY = 19,
        PADDATA_INDEX_DX = 20,
        PADDATA_INDEX_DY = 21,
        PADDATA_INDEX_R = 22,
        PADDATA_INDEX_CHECKSUM = 23,
        PANELDATA_INDEX_CHECKSUM_BEGIN = 2,
        PANELDATA_INDEX_CHECKSUM_END = 22,
    };

    explicit TouchPadWidget(QWidget *parent = 0);
    ~TouchPadWidget();

private:
    void initPadData();
    bool isValidMouseEvent(QMouseEvent *event);
    bool isValidClick(const QPoint &point1, const QPoint &point2);
    void adjustCursor(QMouseEvent *event);
    void processMoveEvent();
    void processClickEvent(QMouseEvent *event);

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

private slots:
    void onPadTypeChanged(const QString &type);
    void onMoveTimeout();

private:
    MOUSEEVENT_TYPE m_mouseEventType;
    QString m_mouseEventType2;
    QCursor m_initCursor;
    QPalette m_initPalette;
    TOUCHPAD_TYPE m_padType;
    MOUSEKEY_TYPE m_mouseKeyType;
    QPoint m_preMovePoint;
    QPoint m_curMovePoint;
    QPoint m_prePressPoint;
    QPoint m_preDPressPoint;
    QTimer m_moveTimer;
    bool m_canMove;
    bool m_isDClicked;
    char m_padData[PADDATA_BYTE_TOTALSUM + 1];
};

#endif // TOUCHPADWIDGET_H
