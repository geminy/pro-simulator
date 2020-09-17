#include "configparser/configparser.h"

#include <QDir>
#include <QDomComment>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QDebug>

namespace simulator
{

static ConfigParser CP;

QList<SocketInfo> GetSocketInfo()
{
    return CP.getSocketInfo();
}

QList<HardkeyInfo> GetHardkeyInfo()
{
    return CP.getHardkeyInfo();
}

QList<ConfigLocationInfo> GetConfigLocationInfo()
{
    return CP.getConfigLocationInfo();
}

QList<ConfigModelInfo> GetConfigModelInfo()
{
    return CP.getConfigModelInfo();
}

/**************************************************/

ConfigParser::ConfigParser()
{
    m_appDir = QDir::currentPath();
    m_configDir = m_appDir.append(QDir::separator()).append(CONFIG_BASENAME);

    searchConfigFiles(m_configDir);
    foreach (QString file, m_configFiles) {
        parseConfigFile(file);
    }
}

QList<SocketInfo> ConfigParser::getSocketInfo()
{
    return m_socketInfo;
}

QList<HardkeyInfo> ConfigParser::getHardkeyInfo()
{
    return m_hardkeyInfo;
}

QList<ConfigLocationInfo> ConfigParser::getConfigLocationInfo()
{
    return m_configLocationInfo;
}

QList<ConfigModelInfo> ConfigParser::getConfigModelInfo()
{
    return m_configModelInfo;
}

void ConfigParser::searchConfigFiles(const QString &path)
{
    if (!QFile::exists(path)) {
        return;
    }

    QFileInfo fileinfo(path);
    if (fileinfo.isFile()) {
        if (fileinfo.suffix() == CONFIG_FILESUFFIX) {
            m_configFiles.append(path);
        }
    }
    else if (fileinfo.isDir()) {
        foreach (QFileInfo file, QDir(fileinfo.absoluteFilePath()).entryInfoList()) {
            if (file.fileName() != "." && file.fileName() != "..") {
                searchConfigFiles(file.absoluteFilePath());
            }
        }
    }
}

void ConfigParser::parseConfigFile(const QString &path)
{
    QDomDocument doc("SimulatorConfigDocument");
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }
    if (!doc.setContent(&file)) {
        file.close();
        return;
    }

    QDomElement docElem = doc.documentElement();
    QString rootName = docElem.tagName();
    if (CONFIG_XML_ROOTTAG == rootName) {
        QString type = docElem.attribute(CONFIG_XML_ROOTTYPE).trimmed();
        if (CONFIG_XML_ROOTTYPE_SOCKET == type) {
            parseSocketData(docElem.childNodes());
        }
        else if (CONFIG_XML_ROOTTYPE_HARDKEY == type) {
            parseHardkeyData(docElem.childNodes());
        }
        else if (CONFIG_XML_ROOTTYPE_TOUCH == type) {
            // inner
        }
        else if (CONFIG_XML_ROOTTYPE_INFO == type) {
            // inner
        }
        else if (CONFIG_XML_ROOTTYPE_CONFIG == type) {
            parseConfigData(docElem.childNodes());
        }
    }
}

void ConfigParser::parseSocketData(const QDomNodeList &node)
{
    SocketInfo socketInfo;
    QDomNode elem;
    QString type;
    for (int i = 0; i < node.length(); ++i) {
        elem = node.at(i);
        if (elem.nodeName() != CONFIG_XML_SOCKET_DEVICE) {
            continue;
        }

        type = elem.toElement().attribute(CONFIG_XML_SOCKET_TYPE).trimmed();
        if (CONFIG_XML_SOCKET_ADB == type) {
            socketInfo.type = type;
            QDomNodeList adbElemList = elem.childNodes();
            QDomNode adbElem;
            for (int adbI = 0; adbI < adbElemList.length(); ++adbI) {
                adbElem = adbElemList.at(adbI);
                if (CONFIG_XML_SOCKET_ADDRESS == adbElem.nodeName()) {
                    socketInfo.address = adbElem.toElement().text().trimmed();
                    socketInfo.port = adbElem.toElement().attribute(CONFIG_XML_SOCKET_PORT).trimmed();
                    m_socketInfo.append(socketInfo);
                }
            }
        }
        else if (CONFIG_XML_SOCKET_LOCALHOST == type) {
            socketInfo.type = type;
            socketInfo.port = CONFIG_XML_SOCKET_DPORT;
            QList<QHostAddress> hostAddressList = QNetworkInterface::allAddresses();
            foreach (QHostAddress addr, hostAddressList) {
                if (addr.protocol() == QAbstractSocket::IPv4Protocol) {
                    socketInfo.address = addr.toString();
                    m_socketInfo.append(socketInfo);
                }
            }
        }
    }
}

void ConfigParser::parseHardkeyData(const QDomNodeList &node)
{
    if (node.length() != 1) {
        return;
    }

    QDomNode hardkey = node.at(0);
    if (hardkey.nodeName() != CONFIG_XML_HARDKEY) {
        return;
    }

    HardkeyInfo info;
    info.kindName = hardkey.toElement().attribute(CONFIG_XML_HARDKEY_NAME).trimmed();
    info.nodeCount = QString(hardkey.toElement().attribute(CONFIG_XML_HARDKEY_COUNT).trimmed()).toInt();
    info.automode = hardkey.toElement().attribute(CONFIG_XML_HARDKEY_AUTOMODE).trimmed();
    if (info.automode == QString(CONFIG_XML_HARDKEY_AUTOMODE)) {
        info.width = QString(hardkey.toElement().attribute(CONFIG_XML_HARDKEY_WIDTH).trimmed()).toInt();
        info.height = QString(hardkey.toElement().attribute(CONFIG_XML_HARDKEY_HEIGHT).trimmed()).toInt();
    }

    HardkeyInfo::HardkeyNode hdkNode;
    QDomNodeList keyList = hardkey.childNodes();
    QDomNode key;
    for (int i = 0; i < keyList.length(); ++i) {
        key = keyList.at(i);
        if (key.nodeName() != QString(CONFIG_XML_HARDKEY_KEY)) {
            continue;
        }

        hdkNode.value = key.toElement().attribute(CONFIG_XML_HARDKEY_VALUE).trimmed();
        hdkNode.strByte = key.toElement().attribute(CONFIG_XML_HARDKEY_BYTE).trimmed();
        hdkNode.byte = hdkNode.strByte.toInt(0, 16);
        if (info.automode != QString(CONFIG_XML_HARDKEY_AUTOMODE)) {
            hdkNode.x = QString(key.toElement().attribute(CONFIG_XML_HARDKEY_X).trimmed()).toInt();
            hdkNode.y = QString(key.toElement().attribute(CONFIG_XML_HARDKEY_Y).trimmed()).toInt();
            hdkNode.width = QString(key.toElement().attribute(CONFIG_XML_HARDKEY_WIDTH).trimmed()).toInt();
            hdkNode.height = QString(key.toElement().attribute(CONFIG_XML_HARDKEY_HEIGHT).trimmed()).toInt();
        }

        info.nodeList.append(hdkNode);
    }

    m_hardkeyInfo.append(info);
}

void ConfigParser::parseConfigData(const QDomNodeList &node)
{
    QDomNode elem;
    QString type;
    for (int i = 0; i < node.length(); ++i) {
        elem = node.at(i);
        type = elem.nodeName();
        if (CONFIG_XML_CONFIG_LOCATION == type) {
            ConfigLocationInfo locationInfo;
            QDomNodeList locationElemList = elem.childNodes();
            QDomNode locationElem;
            for (int locationI = 0; locationI < locationElemList.length(); ++locationI) {
                locationElem = locationElemList.at(locationI);
                if (CONFIG_XML_CONFIG_REGION == locationElem.nodeName()) {
                    int subLen = locationElem.childNodes().length();
                    if (subLen > 0) {
                        locationInfo.hasChild = true;
                        locationInfo.strHByte = locationElem.toElement().attribute(CONFIG_XML_CONFIG_HKEY);
                        locationInfo.value = locationElem.toElement().attribute(CONFIG_XML_CONFIG_VALUE);
                        ConfigLocationInfo subLocationInfo;
                        subLocationInfo.hasChild = false;
                        subLocationInfo.strHByte = locationInfo.strHByte;
                        QDomNode subElem;
                        locationInfo.children.clear();
                        for (int subI = 0; subI < subLen; ++subI) {
                            subElem = locationElem.childNodes().at(subI);
                            if (CONFIG_XML_CONFIG_SUBREGION == subElem.nodeName()) {
                                subLocationInfo.strLByte = subElem.toElement().attribute(CONFIG_XML_CONFIG_LKEY);
                                subLocationInfo.byte = QString(subLocationInfo.strHByte + subLocationInfo.strLByte).toInt(0, 2);
                                subLocationInfo.value = subElem.toElement().attribute(CONFIG_XML_CONFIG_VALUE);
                                locationInfo.children.append(subLocationInfo);
                            }
                        }
                        m_configLocationInfo.append(locationInfo);
                    }
                    else {
                        locationInfo.hasChild = false;
                        locationInfo.strHByte = locationElem.toElement().attribute(CONFIG_XML_CONFIG_HKEY);
                        locationInfo.strLByte = QString("0000");
                        locationInfo.byte = QString(locationInfo.strHByte + locationInfo.strLByte).toInt(0, 2);
                        locationInfo.value = locationElem.toElement().attribute(CONFIG_XML_CONFIG_VALUE);
                        m_configLocationInfo.append(locationInfo);
                    }
                }
            }
        }
        else if (CONFIG_XML_CONFIG_MODEL == type) {
            ConfigModelInfo modelInfo;
            QDomNodeList modelElemList = elem.childNodes();
            QDomNode modelElem;
            for (int modelI = 0; modelI < modelElemList.length(); ++modelI) {
                modelElem = modelElemList.at(modelI);
                if (CONFIG_XML_CONFIG_SYSTEM == modelElem.nodeName()) {
                    modelInfo.strByte = modelElem.toElement().attribute(CONFIG_XML_CONFIG_KEY);
                    modelInfo.byte = modelInfo.strByte.toInt(0, 16);
                    modelInfo.value = modelElem.toElement().attribute(CONFIG_XML_CONFIG_VALUE);
                    m_configModelInfo.append(modelInfo);
                }
            }
        }
    }
}

} // namespace simulator
