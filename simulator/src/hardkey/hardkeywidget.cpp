#include "ui_hardkey.h"
#include "hardkey/hardkeywidget.h"
#include "configparser/configinfo.h"
#include "configparser/configparser.h"

#include <QGridLayout>
#include <QDebug>

HardkeyWidget::HardkeyWidget(QWidget *parent)
    : QWidget(parent)
    , m_pHardkey(new Ui::Hardkey)
{
    setVisible(false);
    m_pHardkey->setupUi(this);
    initConfigData();
}

/* QObjectPrivate::deleteChildren */
HardkeyWidget::~HardkeyWidget()
{
}

void HardkeyWidget::initConfigData()
{
    QList<simulator::HardkeyInfo> hardkeyInfoList = simulator::GetHardkeyInfo();
    foreach (simulator::HardkeyInfo hardkeyInfo, hardkeyInfoList) {
        /* Number */
        if (hardkeyInfo.kindName == QString(CONFIG_XML_HARDKEY_NUMBER)) {
            if (hardkeyInfo.automode == QString(CONFIG_XML_HARDKEY_AUTOMODE)) {
                /* auto mode with grid layout */
                QGridLayout *numberLayout = new QGridLayout;
                /* column x row = grid - 3 x 4 = 12 */
                numberLayout->setColumnMinimumWidth(3 - 1, hardkeyInfo.width);
                foreach (simulator::HardkeyInfo::HardkeyNode hardkeyNode, hardkeyInfo.nodeList) {
                    HardkeyButton *button = new HardkeyButton;
                    button->setCommonData(hardkeyNode.value, hardkeyNode.byte, hardkeyInfo.width, hardkeyInfo.height);
                    numberLayout->addWidget(button);
                }
                m_pHardkey->number->setLayout(numberLayout);
            }
        }
        /* Alphabet */
        else if (hardkeyInfo.kindName == QString(CONFIG_XML_HARDKEY_ALPHABET)) {
            if (hardkeyInfo.automode != QString(CONFIG_XML_HARDKEY_AUTOMODE)) {
                foreach (simulator::HardkeyInfo::HardkeyNode hardkeyNode, hardkeyInfo.nodeList) {
                    /* non-auto mode with specific position */
                    HardkeyButton *button = new HardkeyButton(m_pHardkey->alphabet);
                    button->setCommonData(hardkeyNode.value, hardkeyNode.byte, hardkeyNode.width, hardkeyNode.height);
                    button->setPosition(hardkeyNode.x, hardkeyNode.y);
                }
            }
        }
        /* others */
        else {
            QString otherName = hardkeyInfo.kindName;
            QWidget *otherWidget = new QWidget;
            m_pHardkey->hardkeyTab->addTab(otherWidget, otherName);
            /* non-auto mode with specific position */
            if (hardkeyInfo.automode != QString(CONFIG_XML_HARDKEY_AUTOMODE)) {
                foreach (simulator::HardkeyInfo::HardkeyNode hardkeyNode, hardkeyInfo.nodeList) {
                    HardkeyButton *button = new HardkeyButton(otherWidget);
                    button->setCommonData(hardkeyNode.value, hardkeyNode.byte, hardkeyNode.width, hardkeyNode.height);
                    button->setPosition(hardkeyNode.x, hardkeyNode.y);
                }
            }
        }
    }
}

/* HardkeyButton */
HardkeyButton::HardkeyButton(QWidget *parent)
    : QPushButton(parent)
{
    connect(this, SIGNAL(clicked(bool)), this, SLOT(onClicked(bool)));
 }

HardkeyButton::~HardkeyButton()
{
    disconnect(this, SIGNAL(clicked(bool)), this, SLOT(onClicked(bool)));
}

void HardkeyButton::setCommonData(const QString &name, int byte, int width, int height)
{
    setText(name);
    setFixedSize(width, height);
    m_byte = byte;
}

void HardkeyButton::setPosition(int x, int y)
{
    setGeometry(x, y, width(), height());
}

void HardkeyButton::onClicked(bool checked)
{
    Q_UNUSED(checked);

    QString statusTip("Hardkey: ");
    statusTip.append(text());
    statusTip.append(" ");
    statusTip.append("Value: ");
    statusTip.append(QString::number(m_byte));

    setStatusTip(statusTip);
}
