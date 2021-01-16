import QtQuick 2.11
import QtQuick.Controls 2.3
import Qt.labs.platform 1.1

ApplicationWindow {
    id: window
    Item {
        id: it_connect
        objectName: "item_connect"
        signal connectSN(string community,string key )
        signal disconnectSN()
        signal oauth()
    }

    SystemTrayIcon {
        id:trayIcon
        visible: true
        iconSource: "qrc:/images/AppIcon.png"
        tooltip: "OmniEdge: Connect without corcern"
        Component.onCompleted: showMessage("OmniEdge", "Connect without corcern.")

        menu: Menu {
            id:sysmenu
            MenuItem {
                id:status
                text: qsTr("My address:10.254.1.2")
            }
            MenuSeparator{}
            MenuItem {
                id:connectMenu
                text: qsTr("Connect")
                onTriggered: {
                    disconnectMenu.enabled = true
                    connectMenu.enabled = false
                    it_connect.connectSN("omniedge","66YRd88kyYdhzk")
                }
            }
            MenuItem {
                id:disconnectMenu
                text: qsTr("Disconnect")
                enabled: false
                onTriggered: {
                    connectMenu.enabled = true
                    disconnectMenu.enabled = false
                    it_connect.disconnectSN()
                }
            }
            MenuSeparator{}
            MenuItem {
                text: qsTr("Dashboard")
                onTriggered: Qt.openUrlExternally("http:omniedge.io")
            }
            MenuSeparator{}
            Menu {
                title: "Network Devices"
                MenuItem {
                    text: qsTr("10.254.1.1"+"   ping:30ms")
                }
                MenuItem {
                    text: qsTr("10.254.1.2")
                }
                MenuItem {
                    text: qsTr("10.254.1.3")
                }
            }
            MenuSeparator{}
            MenuItem {
                text: qsTr("Log in...")
                onTriggered:
                    it_connect.oauth()
            }
            MenuItem {
                text: qsTr("Log out")
                //enabled: false
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
                shortcut: "Ctrl+X"
                onTriggered:{
                    Qt.quit()
                    }
                }
            }
    }
}
