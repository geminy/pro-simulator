#include "config/configwidget.h"
#include "configparser/configparser.h"
#include "socket/socket.h"

#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QBoxLayout>
#include <QFrame>
#include <QDebug>

ConfigWidget::ConfigWidget(QWidget *parent)
    : QWidget(parent)
    , m_pRegionLabel(new QLabel("Region"))
    , m_pRegionBox(new QComboBox)
    , m_pSubregionLabel(new QLabel("Subregion"))
    , m_pSubregionBox(new QComboBox)
    , m_pModelLabel(new QLabel("Model"))
    , m_pModelBox(new QComboBox)
    , m_pOkBtn(new QPushButton("OK"))
{
    setVisible(false);
    setFixedSize(CONFIG_WIDGET_WIDTH, CONFIG_WIDGET_HIGHT);
    setStatusTip("Config");

    QHBoxLayout *regionLay = new QHBoxLayout;
    m_pRegionBox->setFixedWidth(CONFIG_COMBOBOX_WIDTH);
    regionLay->addWidget(m_pRegionLabel);
    regionLay->addWidget(m_pRegionBox);
    QHBoxLayout *subLay = new QHBoxLayout;
    m_pSubregionBox->setFixedWidth(CONFIG_COMBOBOX_WIDTH);
    subLay->addWidget(m_pSubregionLabel);
    subLay->addWidget(m_pSubregionBox);
    QHBoxLayout *modelLay = new QHBoxLayout;
    m_pModelBox->setFixedWidth(CONFIG_COMBOBOX_WIDTH);
    modelLay->addWidget(m_pModelLabel);
    modelLay->addWidget(m_pModelBox);
    QHBoxLayout *btnLay = new QHBoxLayout;
    m_pOkBtn->setFixedWidth(CONFIG_OK_WIDTH);
    btnLay->addSpacing(CONFIG_OK_XSPACE);
    btnLay->addWidget(m_pOkBtn);

    QVBoxLayout *bodyLay = new QVBoxLayout;
    bodyLay->addLayout(regionLay);
    bodyLay->addLayout(subLay);
    bodyLay->addLayout(modelLay);
    bodyLay->addLayout(btnLay);

    QFrame *bodyFrame = new QFrame;
    bodyFrame->setFrameShape(QFrame::Panel);
    bodyFrame->setFrameShadow(QFrame::Raised);
    bodyFrame->setLineWidth(CONFIG_FRAME_LINEWIDTH);
    bodyFrame->setFixedSize(CONFIG_FRAME_WIDTH, CONFIG_FRAME_HEIGHT);
    bodyFrame->setLayout(bodyLay);

    QHBoxLayout *spaceLay = new QHBoxLayout;
    spaceLay->addWidget(bodyFrame);

    setLayout(spaceLay);

    initConfigData();
    cacheConfigData();

    connect(m_pRegionBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onRegionTextChanged(QString)));
    connect(m_pOkBtn, SIGNAL(clicked(bool)), this, SLOT(onOkBtnClicked(bool)));
}

/* QObjectPrivate::deleteChildren */
ConfigWidget::~ConfigWidget()
{
    disconnect(m_pRegionBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onRegionTextChanged(QString)));
    disconnect(m_pOkBtn, SIGNAL(clicked(bool)), this, SLOT(onOkBtnClicked(bool)));
}

void ConfigWidget::initConfigData()
{
    memset(m_configData, 0, sizeof(m_configData));

    // head 0~14
    m_configData[0] = 0X9F;
    m_configData[1] = 0X02;
    m_configData[2] = 0X01;
    m_configData[3] = 0X20;
    m_configData[4] = 0X01;
    m_configData[5] = 0X20;
    m_configData[6] = 0XF7;
    m_configData[7] = 0XF8;
    m_configData[8] = 0X00;
    m_configData[9] = 0X84;
    m_configData[10] = 0X00;
    m_configData[11] = 0X00;
    m_configData[12] = 0X00;
    m_configData[13] = 0X00;
    m_configData[14] = 0X10;

    // tail 144~145
    m_configData[144] = 0X9F;
    m_configData[145] = 0X03;
}

void ConfigWidget::cacheConfigData()
{
    if (m_regionCache.isEmpty()) {
        QList<simulator::ConfigLocationInfo> locationList = simulator::GetConfigLocationInfo();
        foreach (simulator::ConfigLocationInfo region, locationList) {
            m_regionCache[region.value] = region;
            m_pRegionBox->addItem(region.value);
        }
    }

    if (m_modelCache.isEmpty()) {
        QList<simulator::ConfigModelInfo> modelList = simulator::GetConfigModelInfo();
        foreach (simulator::ConfigModelInfo model, modelList) {
            m_modelCache[model.value] = model.byte;
            m_pModelBox->addItem(model.value);
        }
    }
}

void ConfigWidget::onRegionTextChanged(const QString &text)
{
    m_pSubregionBox->clear();
    m_subregionCache.clear();

    if (!m_regionCache.isEmpty() && m_regionCache[text].hasChild) {
        foreach (simulator::ConfigLocationInfo subregion, m_regionCache[text].children) {
            m_subregionCache[subregion.value] = subregion;
            m_pSubregionBox->addItem(subregion.value);
        }
    }
}

void ConfigWidget::onOkBtnClicked(bool checked)
{
    Q_UNUSED(checked);

    QString modelValue = m_pModelBox->currentText();
    int modelData = m_modelCache[modelValue];

    int regionData;
    if (m_pSubregionBox->currentText().isEmpty()) {
        regionData = m_regionCache[m_pRegionBox->currentText()].byte;
    }
    else {
        regionData = m_subregionCache[m_pSubregionBox->currentText()].byte;
    }

    m_configData[CONFIGDATA_INDEX_LOCATION] = regionData;
    m_configData[CONFIGDATA_INDEX_MODEL] = modelData;
    for (int i = CONFIGDATA_INDEX_CHECKSUM_BEGIN; i <= CONFIGDATA_INDEX_CHECKSUM_END; ++i) {
        m_configData[CONFIGDATA_INDEX_CHECKSUM] ^= m_configData[i];
    }

    Socket::Instance()->sendData(m_configData, sizeof(m_configData) - 1);
}
