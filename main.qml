import QtQuick 2.11
import QtQuick.Controls 2.12
import Qt.labs.platform 1.1
import QtQml 2.12


Item {
    id: window

    Item {
        id: item_qml
        objectName: "item_qml"
        signal connectSN()
        signal disconnectSN()
        signal login()
        signal loginStatus(bool status)
        signal logout()
        signal wintapError()
        signal configError()
        signal showMsg(string title,string msg)

        onWintapError:{
            trayIcon.show("Wintap Error","Please check if you have install wintap driver")
        }
        onConfigError:{
            trayIcon.show("Config Error","Please check your config")
        }
        onShowMsg:{
            trayIcon.showMessage(title,msg)
        }
        onLoginStatus: {
            sysmenu.isLogin = status
        }
    }

    SystemTrayIcon {
        id:trayIcon
        visible: true
        iconSource: "qrc:/images/AppIcon.png"
        tooltip: "OmniEdge: Connect without corcern"
        Component.onCompleted: {
            showMessage("OmniEdge", "Connect without corcern.")
        }
        menu: Menu {
            id:sysmenu
            property var isLogin: false
            MenuItem {
                text: qsTr("Status: Starting Application...")
                visible: loading
            }
            MenuItem {
                id:statusMenuItem
                text: qsTr("My address:"+myVirtualIP)
                visible: sysmenu.isLogin && !loading
            }
            MenuSeparator{
                visible: sysmenu.isLogin && !loading
            }
            MenuItem {
                id:connectMenuItem
                text: qsTr("Connect")
                visible: sysmenu.isLogin && !loading
                onTriggered: {
                    disconnectMenuItem.enabled = true
                    connectMenuItem.enabled = false
                    item_qml.connectSN()
                }
            }
            MenuItem {
                id:disconnectMenuItem
                text: qsTr("Disconnect")
                visible: sysmenu.isLogin && !loading
                enabled: false
                onTriggered: {
                    connectMenuItem.enabled = true
                    disconnectMenuItem.enabled = false
                    item_qml.disconnectSN()
                }
            }
            MenuSeparator{
                visible: sysmenu.isLogin && !loading
            }
            MenuItem {
                id: dashboardMenuItem
                text: qsTr("Dashboard")
                onTriggered: Qt.openUrlExternally("http://dashboard.omniedge.io")
                visible: sysmenu.isLogin && !loading
            }
            MenuSeparator{
                visible: sysmenu.isLogin && !loading
            }
            Menu {
                id: networkDevicesMenuItem
                title: "Network Devices"
                visible: sysmenu.isLogin && !loading
                Instantiator {
                    model: virtualNetworks
                    MenuItem {
                       id:deviceMenu
                       text: modelData.ipPrefix
                    }
                    // The trick is on those two lines
                    onObjectAdded: networkDevicesMenuItem.insertItem(index, object)
                    onObjectRemoved: networkDevicesMenuItem.removeItem(object)
                }
            }
            MenuSeparator{
                visible: sysmenu.isLogin && !loading
            }
            MenuItem {
                id:loginMenuItem
                text: qsTr("Log in...")
                visible: !sysmenu.isLogin && !loading
                onTriggered:{
                    item_qml.login()
                }
            }
            MenuItem {
                id:logoutMenuItem
                text: qsTr("Logout" + (user ? " as " + user.email : ""))
                visible: sysmenu.isLogin && !loading
                onTriggered:
                {
                    item_qml.logout()
                }
            }
            MenuSeparator{}
            Menu {
                title: "About..."
                MenuItem {
                    text: qsTr("V1.0.0")
                }
            }
            MenuSeparator{}
            MenuItem {
                text: qsTr("Mininize")
                onTriggered:{
                    sysmenu.close()
                }
            }
            MenuItem {
                text: qsTr("Quit")
                onTriggered:{
                    Qt.quit()
                }
            }
        }
    }

}
