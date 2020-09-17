#include "touch/touchpadwidget.h"
#include "touch/touchwidget.h"
#include "socket/socket.h"

#include <QMouseEvent>
#include <QPalette>
#include <QDebug>

#include <cmath>

#define MOUSECLICKED "Click"
#define MOUSEDOUBLECLICKED "DoubleClick"
#define MOUSETOUCHED "Touch"
#define MOUSEPRESSED "Press"
#define MOUSEUNKNOWN "Unknown"

#define VALID_PIXEL (256)
#define VALID_DISTANCE (30)
#define VALID_OFFSET_TIME (20)

TouchPadWidget::TouchPadWidget(QWidget *parent)
    : QWidget(parent)
    , m_mouseEventType(MOUSE_UNKNOWN)
    , m_mouseEventType2(MOUSEUNKNOWN)
    , m_initCursor(cursor())
    , m_initPalette(palette())
    , m_padType(PAD_INVALID)
    , m_mouseKeyType(MOUSE_NONE)
    , m_preMovePoint(QPoint())
    , m_curMovePoint(QPoint())
    , m_prePressPoint(QPoint())
    , m_preDPressPoint(QPoint())
    , m_canMove(false)
    , m_isDClicked(false)
{
    setAutoFillBackground(true);
    initPadData();
    m_moveTimer.setInterval(VALID_OFFSET_TIME);

    connect(TouchWidget::Instance(), SIGNAL(padTypeChanged(QString)), this, SLOT(onPadTypeChanged(QString)));
    connect(&m_moveTimer, SIGNAL(timeout()), this, SLOT(onMoveTimeout()));
}

TouchPadWidget::~TouchPadWidget()
{
}

void TouchPadWidget::initPadData()
{
    memset(m_padData, 0, sizeof(m_padData));

    // head 0~14
    m_padData[0] = 0X9F;
    m_padData[1] = 0X02;
    m_padData[2] = 0X01;
    m_padData[3] = 0X20;
    m_padData[4] = 0X01;
    m_padData[5] = 0X20;
    m_padData[6] = 0XF3;
    m_padData[7] = 0X24;
    m_padData[8] = 0X00;
    m_padData[9] = 0X0D;
    m_padData[10] = 0X20;
    m_padData[11] = 0X00;
    m_padData[12] = 0X00;
    m_padData[13] = 0X00;
    m_padData[14] = 0X78;

    // tail 24~25
    m_padData[24] = 0X9F;
    m_padData[25] = 0X03;
}

bool TouchPadWidget::isValidMouseEvent(QMouseEvent *event)
{
    if (!event) {
        return false;
    }
    /* mouse left/right button accepted */
    if (Qt::LeftButton == event->button()) {
        m_mouseKeyType = MOUSE_LEFT;
    }
    else if (Qt::RightButton == event->button()) {
        m_mouseKeyType = MOUSE_RIGHT;
    }
    else if (Qt::MiddleButton == event->button()) {
        m_mouseKeyType = MOUSE_MIDDLE;
    }
    else {
        m_mouseKeyType = MOUSE_NONE;
    }

    return (MOUSE_LEFT == m_mouseKeyType || MOUSE_RIGHT == m_mouseKeyType);
}

bool TouchPadWidget::isValidClick(const QPoint &point1, const QPoint &point2)
{
    int xd = (point1.x() - point2.x()) * VALID_PIXEL / width();
    int yd = (point1.y() - point2.y()) * VALID_PIXEL / height();
    int zd = sqrt(xd * xd + yd * yd);
    if (zd <= VALID_DISTANCE) {
        return true;
    }
    return false;
}

void TouchPadWidget::adjustCursor(QMouseEvent *event)
{
    /* cursor */
    QPoint point = event->pos();
    bool updateCursor = false;
    int cursorX = point.x();
    int cursorY = point.y();
    if (cursorX <  0) {
        cursorX = 0;
        updateCursor = true;
    }
    else if (cursorX > width()) {
        cursorX = width();
        updateCursor = true;
    }
    if (cursorY <  0) {
        cursorY = 0;
        updateCursor = true;
    }
    else if (cursorY > height()) {
        cursorY = height();
        updateCursor = true;
    }
    if (updateCursor) {
        cursor().setPos(mapToGlobal(QPoint(cursorX, cursorY)));
    }
}

void TouchPadWidget::processMoveEvent()
{
    if (MOUSE_MOVE != m_mouseEventType) {
        return;
    }

    initPadData();
    int xOffset = 0;
    int yOffset = 0;
    int xPos = 0;
    int yPos = 0;

    if (QString(MOUSETOUCHED) == m_mouseEventType2) {
        m_padData[PADDATA_INDEX_COORD] |= COORD_TOUCH;
    }
    else if (QString(MOUSEPRESSED) == m_mouseEventType2) {
        m_padData[PADDATA_INDEX_COORD] |= COORD_PRESS;
    }

    m_padData[PADDATA_INDEX_COORD] |= COORD_PRESS_P1;

    if (PAD_RELATIVE == m_padType) {
        m_padData[PADDATA_INDEX_COORD] |= COORD_RELATIVE;

        xOffset = abs((m_curMovePoint.x() - m_preMovePoint.x()) * VALID_PIXEL / width());
        yOffset = abs((m_curMovePoint.y() - m_preMovePoint.y()) * VALID_PIXEL / height());
        m_padData[PADDATA_INDEX_VX] = xOffset;
        m_padData[PADDATA_INDEX_VY] = yOffset;

        if (m_curMovePoint.x() > m_preMovePoint.x()) {
            m_padData[PADDATA_INDEX_DX] =OFFSET_X_RIGHT;
        }
        else if (m_curMovePoint.x() < m_preMovePoint.x()) {
            m_padData[PADDATA_INDEX_DX] =OFFSET_X_LEFT;
        }
        if (m_curMovePoint.y() > m_preMovePoint.y()) {
            m_padData[PADDATA_INDEX_DY] =OFFSET_Y_DOWN;
        }
        else if (m_curMovePoint.y() < m_preMovePoint.y()) {
            m_padData[PADDATA_INDEX_DY] =OFFSET_Y_UP;
        }
    }
    else if (PAD_ABSOLUTE == m_padType) {
        m_padData[PADDATA_INDEX_COORD] |= COORD_ABSOLUTE;

        xPos = m_curMovePoint.x() * VALID_PIXEL / width();
        yPos = m_curMovePoint.y() * VALID_PIXEL / height();
        m_padData[PADDATA_INDEX_VX] = xPos;
        m_padData[PADDATA_INDEX_VY] = yPos;
    }

    if (PAD_RELATIVE == m_padType) {
        TouchWidget::Instance()->showTouchState(m_mouseEventType2, true, QPoint(xOffset, yOffset));
    }
    else if (PAD_ABSOLUTE == m_padType) {
        TouchWidget::Instance()->showTouchState(m_mouseEventType2, true, QPoint(xPos, yPos));
    }

    for (int i = PANELDATA_INDEX_CHECKSUM_BEGIN; i <= PANELDATA_INDEX_CHECKSUM_END; ++i) {
        m_padData[PADDATA_INDEX_CHECKSUM] ^= m_padData[i];
    }

    Socket::Instance()->sendData(m_padData, sizeof(m_padData) - 1);
}

void TouchPadWidget::processClickEvent(QMouseEvent *event)
{
    if (MOUSE_PRESS != m_mouseEventType && MOUSE_RELEASE != m_mouseEventType && MOUSE_DOUBLECLICK != m_mouseEventType) {
        return;
    }

    initPadData();
    QPoint curPoint = event->pos();
    int xp = 0;
    int yp = 0;

    if (MOUSE_PRESS == m_mouseEventType) {
        m_prePressPoint = curPoint;
        return;
    }
    else if (MOUSE_RELEASE == m_mouseEventType) {
        if (m_isDClicked) {
            if (!isValidClick(m_preDPressPoint, curPoint)) {
                return;
            }
            /* real double click */
            m_mouseEventType2 = MOUSEDOUBLECLICKED;

            m_padData[PADDATA_INDEX_COORD] |= COORD_DOUBLECLICK;

            if (PAD_RELATIVE == m_padType) {
                m_padData[PADDATA_INDEX_COORD] |= COORD_RELATIVE;
            }
            else if (PAD_ABSOLUTE == m_padType) {
                m_padData[PADDATA_INDEX_COORD] |= COORD_ABSOLUTE;

                xp = m_prePressPoint.x() * VALID_PIXEL / width();
                yp = m_prePressPoint.y() * VALID_PIXEL / height();
                m_padData[PADDATA_INDEX_VX] = xp;
                m_padData[PADDATA_INDEX_VY] = yp;
            }
        }
        else {
            if (!isValidClick(m_prePressPoint, curPoint)) {
                return;
            }
            /* real click */
            m_mouseEventType2 = MOUSECLICKED;

            m_padData[PADDATA_INDEX_COORD] |= COORD_CLICK;

            if (PAD_RELATIVE == m_padType) {
                m_padData[PADDATA_INDEX_COORD] |= COORD_RELATIVE;
            }
            else if (PAD_ABSOLUTE == m_padType) {
                m_padData[PADDATA_INDEX_COORD] |= COORD_ABSOLUTE;

                xp = m_prePressPoint.x() * VALID_PIXEL / width();
                yp = m_prePressPoint.y() * VALID_PIXEL / height();
                m_padData[PADDATA_INDEX_VX] = xp;
                m_padData[PADDATA_INDEX_VY] = yp;
            }
        }
    }
    else if (MOUSE_DOUBLECLICK == m_mouseEventType) {
        if (!isValidClick(m_prePressPoint, curPoint)) {
            return;
        }
        /* maybe double click */
        m_isDClicked = true;
        m_preDPressPoint = curPoint;
    }

    for (int i = PANELDATA_INDEX_CHECKSUM_BEGIN; i <= PANELDATA_INDEX_CHECKSUM_END; ++i) {
        m_padData[PADDATA_INDEX_CHECKSUM] ^= m_padData[i];
    }

    TouchWidget::Instance()->showTouchState(m_mouseEventType2, true, QPoint(xp, yp));

    Socket::Instance()->sendData(m_padData, sizeof(m_padData) - 1);
}

void TouchPadWidget::mousePressEvent(QMouseEvent *event)
{
    if (!isValidMouseEvent(event)) {
        return;
    }
    if (PAD_RELATIVE == m_padType) {
        setPalette(QPalette(Qt::gray));
        setCursor(Qt::PointingHandCursor);
    }
    m_mouseEventType = MOUSE_PRESS;
    adjustCursor(event);
    processClickEvent(event);

    QWidget::mousePressEvent(event);
}

void TouchPadWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (!isValidMouseEvent(event)) {
        return;
    }
    if (PAD_RELATIVE == m_padType) {
        setPalette(m_initPalette);
        setCursor(m_initCursor);
    }
    m_mouseEventType = MOUSE_RELEASE;
    adjustCursor(event);
    processClickEvent(event);
    m_isDClicked = false;

    QWidget::mouseReleaseEvent(event);
}

void TouchPadWidget::mouseMoveEvent(QMouseEvent *event)
{
    /* ignores mouse middle button event */
    if (MOUSE_NONE == m_mouseKeyType || MOUSE_MIDDLE == m_mouseKeyType) {
        return;
    }
    m_mouseEventType = MOUSE_MOVE;
    if (MOUSE_LEFT == m_mouseKeyType) {
        m_mouseEventType2 = MOUSETOUCHED;
    }
    else if (MOUSE_RIGHT == m_mouseKeyType) {
        m_mouseEventType2 = MOUSEPRESSED;
    }
    adjustCursor(event);

    /* move: touch/press */
    if (PAD_RELATIVE == m_padType) {
        if (m_canMove) {
            m_curMovePoint = event->pos();
            processMoveEvent();
            m_preMovePoint = m_curMovePoint;
            m_canMove = false;
        }
        else {
            if (!m_moveTimer.isActive()) {
                m_preMovePoint = event->pos();
                m_moveTimer.start();
            }
        }
    }
    else if (PAD_ABSOLUTE == m_padType) {
        m_curMovePoint = event->pos();
        processMoveEvent();
    }

    QWidget::mouseMoveEvent(event);
}

void TouchPadWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (!isValidMouseEvent(event)) {
        return;
    }
    m_mouseEventType = MOUSE_DOUBLECLICK;
    adjustCursor(event);
    processClickEvent(event);

    QWidget::mouseDoubleClickEvent(event);
}

void TouchPadWidget::onPadTypeChanged(const QString &type)
{
    if (QString("Relative") == type) {
        m_padType = PAD_RELATIVE;
    }
    else if (QString("Absolute") == type) {
        m_padType = PAD_ABSOLUTE;
    }
}

void TouchPadWidget::onMoveTimeout()
{
    m_canMove = true;
}
