#include "mainwindow/homewidget.h"
#include "configparser/configinfo.h"

#include <QPushButton>
#include <QBoxLayout>

HomeWidget::HomeWidget(QWidget *parent)
    : QWidget(parent)
    , m_pHardkey(new QPushButton(HOME_WIDGET_HARDKEY))
    , m_pTouch(new QPushButton(HOME_WIDGET_TOUCH))
    , m_pInfo(new QPushButton(HOME_WIDGET_INFO))
    , m_pConfig(new QPushButton(HOME_WIDGET_CONFIG))
{
    setVisible(false);

    setFixedSize(HOME_WIDGET_WIDTH, HOME_WIDGET_HIGHT);
    m_pHardkey->setFixedSize(HOME_BUTTON_WIDTH, HOME_BUTTON_HEIGHT);
    m_pTouch->setFixedSize(HOME_BUTTON_WIDTH, HOME_BUTTON_HEIGHT);
    m_pInfo->setFixedSize(HOME_BUTTON_WIDTH, HOME_BUTTON_HEIGHT);
    m_pConfig->setFixedSize(HOME_BUTTON_WIDTH, HOME_BUTTON_HEIGHT);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(m_pHardkey);
    layout->addWidget(m_pTouch);
    QHBoxLayout *layout2 = new QHBoxLayout;
    layout2->addWidget(m_pInfo);
    layout2->addWidget(m_pConfig);
    QVBoxLayout *layout3 = new QVBoxLayout;
    layout3->addLayout(layout);
    layout3->addLayout(layout2);
    setLayout(layout3);

    connect(m_pHardkey, SIGNAL(clicked(bool)), this, SIGNAL(hardkeyBtnClicked(bool)));
    connect(m_pTouch, SIGNAL(clicked(bool)), this, SIGNAL(touchBtnClicked(bool)));
    connect(m_pInfo, SIGNAL(clicked(bool)), this, SIGNAL(infoBtnClicked(bool)));
    connect(m_pConfig, SIGNAL(clicked(bool)), this, SIGNAL(configBtnClicked(bool)));
}

/* QObjectPrivate::deleteChildren */
HomeWidget::~HomeWidget()
{
    disconnect(m_pHardkey, SIGNAL(clicked(bool)), this, SIGNAL(hardkeyBtnClicked(bool)));
    disconnect(m_pTouch, SIGNAL(clicked(bool)), this, SIGNAL(touchBtnClicked(bool)));
    disconnect(m_pInfo, SIGNAL(clicked(bool)), this, SIGNAL(infoBtnClicked(bool)));
    disconnect(m_pConfig, SIGNAL(clicked(bool)), this, SIGNAL(configBtnClicked(bool)));
}
