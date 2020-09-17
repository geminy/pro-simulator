#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include "configinfo.h"

#include <QString>
#include <QList>
#include <QDomNodeList>

namespace simulator
{

QList<SocketInfo> GetSocketInfo();
QList<HardkeyInfo> GetHardkeyInfo();
QList<ConfigLocationInfo> GetConfigLocationInfo();
QList<ConfigModelInfo> GetConfigModelInfo();

/**************************************************/

class ConfigParser
{
public:
    ConfigParser();

    QList<SocketInfo> getSocketInfo();
    QList<HardkeyInfo> getHardkeyInfo();
    QList<ConfigLocationInfo> getConfigLocationInfo();
    QList<ConfigModelInfo> getConfigModelInfo();

private:
    void searchConfigFiles(const QString &path);
    void parseConfigFile(const QString &path);
    void parseSocketData(const QDomNodeList &node);
    void parseHardkeyData(const QDomNodeList &node);
    void parseConfigData(const QDomNodeList &node);

private:
    QString m_appDir;
    QString m_configDir;
    QList<QString> m_configFiles;
    QList<SocketInfo> m_socketInfo;
    QList<HardkeyInfo> m_hardkeyInfo;
    QList<ConfigLocationInfo> m_configLocationInfo;
    QList<ConfigModelInfo> m_configModelInfo;
};

} // namespace simulator

#endif // CONFIGPARSER_H
