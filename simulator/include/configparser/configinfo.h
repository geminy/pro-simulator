#ifndef CONFIGINFO_H
#define CONFIGINFO_H

#include <QString>
#include <QList>

/* mainwindow */
#define WINDOW_SIZE_WIDTH (480)
#define WINDOW_SIZE_HEIGHT (640)
#define WINDOW_TITLE "Simulator"
#define WINDOW_ICON ":/icon/icon.png"

/* menubar */
#define MENUBAR_BEGIN "&Begin"
#define MENUBAR_SEARCH "&Search"
#define MENUBAR_CONNECT "&Connect"
#define MENUBAR_DISCONNECT "&Disconnect"

/* home widget */
#define HOME_WIDGET_WIDTH (WINDOW_SIZE_WIDTH)
#define HOME_WIDGET_HIGHT (WINDOW_SIZE_WIDTH + 100)
#define HOME_WIDGET_HARDKEY "Hardkey"
#define HOME_WIDGET_TOUCH "Touch"
#define HOME_WIDGET_INFO "Info"
#define HOME_WIDGET_CONFIG  "Config"
#define HOME_BUTTON_WIDTH (200)
#define HOME_BUTTON_HEIGHT (200)

/* touch widget */
#define TOUCH_PANEL "Panel"
#define TOUCH_PAD "Pad"
#define TOUCH_PANEL_MAGICWIDTH (1024)
#define TOUCH_PANEL_MAGICHEIGHT (1024)
#define TOUCH_PANEL_NARROW "Narrow"
#define TOUCH_PANEL_WIDE "Wide"
#define TOUCH_NARROW_WIDTH (800)
#define TOUCH_NARROW_HEIGHT (480)
#define TOUCH_WIDE_WIDTH (1280)
#define TOUCH_WIDE_HEIGHT (480)
#define TOUCH_PAD_RELATIVE "Relative"
#define TOUCH_PAD_ABSOLUTE "Absolute"

/* config widget */
#define CONFIG_WIDGET_WIDTH (WINDOW_SIZE_WIDTH)
#define CONFIG_WIDGET_HIGHT (WINDOW_SIZE_WIDTH + 100)
#define CONFIG_FRAME_WIDTH (WINDOW_SIZE_WIDTH - 100)
#define CONFIG_FRAME_HEIGHT (WINDOW_SIZE_WIDTH)
#define CONFIG_FRAME_LINEWIDTH (20)
#define CONFIG_LAYOUT_XSPACE (35)
#define CONFIG_COMBOBOX_WIDTH (220)
#define CONFIG_OK_WIDTH (100)
#define CONFIG_OK_XSPACE (20)

/* back button */
#define BUTTON_BACK "< Back"
#define BUTTON_BACK_WIDTH (50)
#define BUTTON_BACK_HEIGHT (30)

/* xml */
#define CONFIG_BASENAME "config"
#define CONFIG_FILESUFFIX "xml"
#define CONFIG_XML_ROOTTAG "simulator"
#define CONFIG_XML_ROOTTYPE "type"
#define CONFIG_XML_ROOTTYPE_SOCKET "socket"
#define CONFIG_XML_ROOTTYPE_HARDKEY "hardkey"
#define CONFIG_XML_ROOTTYPE_TOUCH "touch"
#define CONFIG_XML_ROOTTYPE_INFO "info"
#define CONFIG_XML_ROOTTYPE_CONFIG "config"

/* socket xml */
#define CONFIG_XML_SOCKET_DEVICE "device"
#define CONFIG_XML_SOCKET_TYPE "type"
#define CONFIG_XML_SOCKET_ADB "adb"
#define CONFIG_XML_SOCKET_LOCALHOST "localhost"
#define CONFIG_XML_SOCKET_ADDRESS "address"
#define CONFIG_XML_SOCKET_PORT "port"
/* socket default port */
#define CONFIG_XML_SOCKET_DPORT "9876"

/* hardkey xml */
#define CONFIG_XML_HARDKEY "hardkey"
#define CONFIG_XML_HARDKEY_NAME "name"
#define CONFIG_XML_HARDKEY_COUNT "count"
#define CONFIG_XML_HARDKEY_AUTOMODE "automode"
#define CONFIG_XML_HARDKEY_X "x"
#define CONFIG_XML_HARDKEY_Y "y"
#define CONFIG_XML_HARDKEY_WIDTH "width"
#define CONFIG_XML_HARDKEY_HEIGHT "height"
#define CONFIG_XML_HARDKEY_KEY "key"
#define CONFIG_XML_HARDKEY_BYTE "byte"
#define CONFIG_XML_HARDKEY_VALUE "value"
#define CONFIG_XML_HARDKEY_NUMBER "Number"
#define CONFIG_XML_HARDKEY_ALPHABET "Alphabet"

/* config xml */
#define CONFIG_XML_CONFIG_LOCATION "location"
#define CONFIG_XML_CONFIG_MODEL "model"
#define CONFIG_XML_CONFIG_REGION "region"
#define CONFIG_XML_CONFIG_SUBREGION "subregion"
#define CONFIG_XML_CONFIG_SYSTEM "system"
#define CONFIG_XML_CONFIG_KEY "key"
#define CONFIG_XML_CONFIG_HKEY "hkey"
#define CONFIG_XML_CONFIG_LKEY "lkey"
#define CONFIG_XML_CONFIG_VALUE "value"

namespace simulator
{

struct SocketInfo
{
    QString type;
    QString address;
    QString port;
};

struct HardkeyInfo
{
    struct HardkeyNode
    {
        int byte;
        QString strByte;
        QString value;
        int x;
        int y;
        int width;
        int height;
    };

    QString kindName;
    int nodeCount;
    QList<HardkeyNode> nodeList;
    QString automode;
    int width;
    int height;
};

struct ConfigLocationInfo
{
    int byte;
    QString strHByte;
    QString strLByte;
    QString value;
    bool hasChild;
    QList<ConfigLocationInfo> children;

    ConfigLocationInfo() : hasChild(false) {}
};

struct ConfigModelInfo
{
    int byte;
    QString strByte;
    QString value;
};

} // namespace simulator

#endif // CONFIGINFO_H
