#include "info/infowidget.h"
#include "configparser/configinfo.h"
#include "socket/socket.h"

#include <QDebug>

InfoWidget::InfoWidget(QWidget *parent)
    : QWidget(parent)
{
    setVisible(false);
    setupUi(this);
    initInfoData();
}

InfoWidget::~InfoWidget()
{
}

void InfoWidget::initInfoData()
{
    memset(m_illumiData, 0, sizeof(m_illumiData));
    memset(m_revData, 0, sizeof(m_revData));

    // head 0~15
    m_illumiData[0] = 0X9F;
    m_illumiData[1] = 0X02;
    m_illumiData[2] = 0X01;
    m_illumiData[3] = 0X20;
    m_illumiData[4] = 0X01;
    m_illumiData[5] = 0X20;
    m_illumiData[6] = 0XF7;
    m_illumiData[7] = 0XF8;
    m_illumiData[8] = 0X00;
    m_illumiData[9] = 0X1D;
    m_illumiData[10] = 0X10;
    m_illumiData[11] = 0X00;
    m_illumiData[12] = 0X00;
    m_illumiData[13] = 0X00;
    m_illumiData[14] = 0X1F;
    m_illumiData[15] = 0X00;
    // tail 41~42
    m_illumiData[41] = 0X9F;
    m_illumiData[42] = 0X03;

    //  head 0~14
    m_revData[0] = 0X9F;
    m_revData[1] = 0X02;
    m_revData[2] = 0X01;
    m_revData[3] = 0X20;
    m_revData[4] = 0X01;
    m_revData[5] = 0X20;
    m_revData[6] = 0XFC;
    m_revData[7] = 0XF8;
    m_revData[8] = 0X00;
    m_revData[9] = 0X09;
    m_revData[10] = 0X80;
    m_revData[11] = 0X00;
    m_revData[12] = 0X00;
    m_revData[13] = 0X00;
    m_revData[14] = 0XF6;
    // tail 39~40
    m_revData[39] = 0X9F;
    m_revData[40] = 0X03;
}

void InfoWidget::on_illumiOkBtn_clicked()
{
    if (illumiStateOn->isChecked()) {
        m_illumiData[INFOILLUMIDATA_INDEX_STATE] = 1;
    }
    else if (illumiStateNone->isChecked()){
        m_illumiData[INFOILLUMIDATA_INDEX_STATE] = (1 << 1);
    }

    m_illumiData[INFOILLUMIDATA_INDEX_LEVEL] = illumiLevelSpinBox->text().toInt();

    if (Qt::Checked == illumiPkb->checkState()) {
        m_illumiData[INFOILLUMIDATA_INDEX_DRIVE] += 1;
    }
    if (Qt::Checked == illumiIg->checkState()) {
        m_illumiData[INFOILLUMIDATA_INDEX_DRIVE] += (1 << 3);
    }
    if (Qt::Checked == illumiRev->checkState()) {
        m_illumiData[INFOILLUMIDATA_INDEX_DRIVE] += (1 << 6);
    }

    m_illumiData[INFOILLUMIDATA_INDEX_SPEED] = illumiSpeedSpinBox->text().toInt();

    if (Qt::Checked == illumiBrakeTest->checkState()) {
        m_illumiData[INFOILLUMIDATA_INDEX_PARK] += 1;
    }
    if (Qt::Checked == illumiBrakePark->checkState()) {
        m_illumiData[INFOILLUMIDATA_INDEX_PARK] += (1 << 1);
    }
    if (Qt::Checked == illumiBrakeP->checkState()) {
        m_illumiData[INFOILLUMIDATA_INDEX_PARK] += (1 << 2);
    }
    if (Qt::Checked == illumiBrakeN->checkState()) {
        m_illumiData[INFOILLUMIDATA_INDEX_PARK] += (1 << 3);
    }

    for (int i = INFOILLUMIDATA_INDEX_CHECKSUM_BEGIN; i <= INFOILLUMIDATA_INDEX_CHECKSUM_END; ++i) {
        m_illumiData[INFOILLUMIDATA_INDEX_CHECKSUM] ^= m_illumiData[i];
    }

    Socket::Instance()->sendData(m_illumiData, sizeof(m_illumiData) - 1);
}

void InfoWidget::on_revOkBtn_clicked()
{
    if (Qt::Checked == revRev->checkState()) {
        m_revData[INFOREVDATA_INDEX_REV] += (1 << 5);
    }
    if (Qt::Checked == revPkb->checkState()) {
        m_revData[INFOREVDATA_INDEX_REV] += (1 << 6);
    }

    m_revData[INFOREVDATA_INDEX_SPEED] = revSpdSpinBox->text().toInt();

    for (int i = INFOREVDATA_INDEX_CHECKSUM_BEGIN; i <= INFOREVDATA_INDEX_CHECKSUM_END; ++i) {
        m_revData[INFOREVDATA_INDEX_CHECKSUM] ^= m_revData[i];
    }

    Socket::Instance()->sendData(m_revData, sizeof(m_revData) - 1);
}
