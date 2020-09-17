#ifndef CONFIGWIDGET_H
#define CONFIGWIDGET_H

#include "configparser/configinfo.h"

#include <QWidget>

class QLabel;
class QComboBox;
class QPushButton;

class ConfigWidget : public QWidget
{
    Q_OBJECT

public:
    enum CONFIGWIDGETG_DATA
    {
        CONFIGDATA_BYTE_HEAD_FIXED = 15,
        CONFIGDATA_BYTE_HEAD_NULL = 14,
        CONFIGDATA_BYTE_LOCATION = 1,
        CONFIGDATA_BYTE_MODEL = 1,
        CONFIGDATA_BYTE_TAIL_NULL = 112,
        CONFIGDATA_BYTE_CHECKSUM = 1,
        CONFIGDATA_BYTE_TAIL_FIXED = 2,
        CONFIGDATA_BYTE_TOTALSUM = 146,

        CONFIGDATA_INDEX_LOCATION = 29,
        CONFIGDATA_INDEX_MODEL = 30,
        CONFIGDATA_INDEX_CHECKSUM = 143,
        CONFIGDATA_INDEX_CHECKSUM_BEGIN = 2,
        CONFIGDATA_INDEX_CHECKSUM_END = 142
    };

    explicit ConfigWidget(QWidget *parent);
    ~ConfigWidget();

private:
    void initConfigData();
    void cacheConfigData();

private slots:
    void onRegionTextChanged(const QString &text);
    void onOkBtnClicked(bool checked = false);

private:
    QLabel *m_pRegionLabel;
    QComboBox *m_pRegionBox;
    QLabel *m_pSubregionLabel;
    QComboBox *m_pSubregionBox;
    QLabel *m_pModelLabel;
    QComboBox *m_pModelBox;
    QPushButton *m_pOkBtn;
    QHash<QString, int> m_modelCache;
    QHash<QString, simulator::ConfigLocationInfo> m_regionCache;
    QHash<QString, simulator::ConfigLocationInfo> m_subregionCache;
    char m_configData[CONFIGDATA_BYTE_TOTALSUM + 1];
};

#endif // CONFIGWIDGET_H
