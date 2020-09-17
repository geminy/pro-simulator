#include "ui_touch.h"
#include "touch/touchwidget.h"
#include "configparser/configinfo.h"
#include "socket/socket.h"

#include <QMouseEvent>
#include <QDebug>

TouchWidget* TouchWidget::s_pSelf = 0;

TouchWidget* TouchWidget::Instance()
{
    return s_pSelf;
}

TouchWidget::TouchWidget(QWidget *parent)
    : QWidget(parent)
    , m_pTouch(new Ui::Touch)
    , m_touchType(TOUCHWIDGET_TOUCH)
    , m_panelType(TOUCHWIDGET_PANEL)
    , m_padType(TOUCHWIDGET_PAD)
    , m_mouseEventType(TOUCHWIDGET_MOUSEEVENT_NONE)
    , m_isBigEndian(false)
{
    s_pSelf = this;

    setVisible(false);

    m_pTouch->setupUi(this);
    on_touchTab_currentChanged(m_pTouch->touchTab->currentIndex());
    on_panelResolution_currentChanged(m_pTouch->panelResolution->currentIndex());
    on_padCoord_currentChanged(m_pTouch->padCoord->currentIndex());

    setEndianState();
    initPanelData();
}

/* QObjectPrivate::deleteChildren */
TouchWidget::~TouchWidget()
{
}

void TouchWidget::showTouchState(const QString &state, bool show, const QPoint &pos)
{
    if (!show) {
        m_pTouch->touchTip->setText(state);
    }
    else {
        QString posTip = QString(" (") + QString::number(pos.x()) + QString(", ") + QString::number(pos.y()) + QString(")");
        m_pTouch->touchTip->setText(state + posTip);
    }
}

void TouchWidget::setEndianState()
{
    union
    {
        char c;
        int n;
    } endian;

    endian.c = 0;
    endian.n = 1;

    if (1 == endian.c) {
        m_isBigEndian = false;
    }
    else {
        m_isBigEndian = true;
    }
}

void TouchWidget::initPanelData()
{
    memset(m_panelData, 0, sizeof(m_panelData));

    // head 0~14
    m_panelData[0] = 0X9F;
    m_panelData[1] = 0X02;
    m_panelData[2] = 0X01;
    m_panelData[3] = 0X20;
    m_panelData[4] = 0X01;
    m_panelData[5] = 0X20;
    m_panelData[6] = 0X21;
    m_panelData[7] = 0X24;
    m_panelData[8] = 0X00;
    m_panelData[9] = 0X0E;
    m_panelData[10] = 0X20;
    m_panelData[11] = 0X00;
    m_panelData[12] = 0X00;
    m_panelData[13] = 0X00;
    m_panelData[14] = 0X7A;

    // count 23
    m_panelData[23] = 0X01;

    // tail 25~26
    m_panelData[25] = 0X9F;
    m_panelData[26] = 0X03;
}

void TouchWidget::parseMouseEvent(QMouseEvent *event)
{
    if (!event) {
        return;
    }

    if (TOUCHWIDGET_PANEL == m_touchType) {
        parsePanelData(event->pos());
    }
}

void TouchWidget::parsePanelData(const QPoint &pos)
{
    if (TOUCHWIDGET_PANEL != m_touchType) {
        return;
    }

    QPoint posi = QPoint();
    int xpos = -1;
    int ypos = -1;

    /* narrow */
    if (TOUCHWIDGET_PANEL_NARROW == m_panelType) {
        if (m_pTouch && m_pTouch->narrowArea) {
            posi = m_pTouch->narrowArea->mapFrom(this, pos);
            xpos = posi.x();
            ypos = posi.y();
            if (xpos < 0 || xpos > m_pTouch->narrowArea->width() || ypos < 0 || ypos > m_pTouch->narrowArea->height()) {
                return;
            }
            // pixel
            xpos *= (TOUCH_PANEL_MAGICWIDTH - 1) / m_pTouch->narrowArea->width();
            ypos *= (TOUCH_PANEL_MAGICHEIGHT - 1) / m_pTouch->narrowArea->height();
            ypos = TOUCH_PANEL_MAGICHEIGHT - 1 - ypos;
        }
        else {
            return;
        }
    }
    /* wide */
    else if (TOUCHWIDGET_PANEL_WIDE == m_panelType) {
        if (m_pTouch && m_pTouch->wideArea) {
            posi = m_pTouch->wideArea->mapFrom(this, pos);
            xpos = posi.x();
            ypos = posi.y();
            if (xpos < 0 || xpos > m_pTouch->wideArea->width() || ypos < 0 || ypos > m_pTouch->wideArea->height()) {
                return;
            }
            // pixel
            xpos *= (TOUCH_PANEL_MAGICWIDTH - 1) / m_pTouch->wideArea->width();
            ypos *= (TOUCH_PANEL_MAGICHEIGHT - 1) / m_pTouch->wideArea->height();
            ypos = TOUCH_PANEL_MAGICHEIGHT - 1 - ypos;
        }
        else {
            return;
        }
    }
    else {
        return;
    }

    /* press and move */
    if (TOUCHWIDGET_MOUSEEVENT_PRESS == m_mouseEventType
            || TOUCHWIDGET_MOUSEEVENT_MOVE == m_mouseEventType) {
        if (m_isBigEndian) {
            m_panelData[PANELDATA_INDEX_HX1] = xpos & (0X00FF0000);
            m_panelData[PANELDATA_INDEX_LX1] = xpos & (0XFF000000);
            m_panelData[PANELDATA_INDEX_HY1] = ypos & (0X00FF0000);
            m_panelData[PANELDATA_INDEX_LY1] = ypos & (0XFF000000);
        }
        else {
            m_panelData[PANELDATA_INDEX_HX1] = xpos & (0X0000FF00);
            m_panelData[PANELDATA_INDEX_LX1] = xpos & (0X000000FF);
            m_panelData[PANELDATA_INDEX_HY1] = ypos & (0X0000FF00);
            m_panelData[PANELDATA_INDEX_LY1] = ypos & (0X000000FF);
        }
    }

    for (int i = PANELDATA_INDEX_CHECKSUM_BEGIN; i <= PANELDATA_INDEX_CHECKSUM_END; ++i) {
        m_panelData[PANELDATA_INDEX_CHECKSUM] ^= m_panelData[i];
    }

    QString state = QString();
    if (TOUCHWIDGET_MOUSEEVENT_PRESS == m_mouseEventType) {
        state = "Pressed";
    }
    else if (TOUCHWIDGET_MOUSEEVENT_RELEASE == m_mouseEventType) {
        state = "Released";
    }
    else if (TOUCHWIDGET_MOUSEEVENT_MOVE == m_mouseEventType) {
        state = "Moved";
    }

    showTouchState(state, true, QPoint(xpos, ypos));
    sendData();
}

void TouchWidget::sendData()
{
    if (TOUCHWIDGET_PANEL == m_touchType) {
        Socket::Instance()->sendData(m_panelData, sizeof(m_panelData) - 1);
    }
}

void TouchWidget::mousePressEvent(QMouseEvent *event)
{
    m_mouseEventType = TOUCHWIDGET_MOUSEEVENT_PRESS;
    parseMouseEvent(event);

    QWidget::mousePressEvent(event);
}

void TouchWidget::mouseReleaseEvent(QMouseEvent *event)
{
    m_mouseEventType = TOUCHWIDGET_MOUSEEVENT_RELEASE;
    parseMouseEvent(event);

    QWidget::mouseReleaseEvent(event);
}

void TouchWidget::mouseMoveEvent(QMouseEvent *event)
{
    m_mouseEventType = TOUCHWIDGET_MOUSEEVENT_MOVE;
    parseMouseEvent(event);

    QWidget::mouseMoveEvent(event);
}

void TouchWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    QWidget::mouseDoubleClickEvent(event);
}

void TouchWidget::on_touchTab_currentChanged(int index)
{
    QString tabText = m_pTouch->touchTab->tabText(index);

    if (tabText.indexOf(QString(TOUCH_PANEL), 0, Qt::CaseInsensitive) != -1) {
        m_touchType = TOUCHWIDGET_PANEL;
    }
    else if (tabText.indexOf(QString(TOUCH_PAD), 0, Qt::CaseInsensitive) != -1) {
        m_touchType = TOUCHWIDGET_PAD;
    }

    showTouchState("");
}

void TouchWidget::on_panelResolution_currentChanged(int index)
{
    QString tabText = m_pTouch->panelResolution->tabText(index);

    if (tabText.indexOf(QString(TOUCH_PANEL_NARROW), 0, Qt::CaseInsensitive) != -1) {
        m_panelType = TOUCHWIDGET_PANEL_NARROW;
    }
    else if (tabText.indexOf(QString(TOUCH_PANEL_WIDE), 0, Qt::CaseInsensitive) != -1) {
        m_panelType = TOUCHWIDGET_PANEL_WIDE;
    }

    showTouchState("");
}

void TouchWidget::on_padCoord_currentChanged(int index)
{
    QString tabText = m_pTouch->padCoord->tabText(index);

    if (tabText.indexOf(QString(TOUCH_PAD_ABSOLUTE), 0, Qt::CaseInsensitive) != -1) {
        m_padType = TOUCHWIDGET_PAD_ABSOLUTE;
        emit padTypeChanged("Absolute");
    }
    else if (tabText.indexOf(QString(TOUCH_PAD_RELATIVE), 0, Qt::CaseInsensitive) != -1) {
        m_padType = TOUCHWIDGET_PAD_RELATIVE;
        emit padTypeChanged("Relative");
    }

    showTouchState("");
}
