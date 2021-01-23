import QtQuick 2.11
import QtQuick.Controls 2.12
import Qt.labs.platform 1.1
import QtQml 2.12


ApplicationWindow {
    id: window

    Item {
        id: item_qml
        objectName: "item_qml"
        signal connectSN(string community,string key)
        signal disconnectSN()
        signal oauth()
        signal logout()
        signal isLogin(bool status)
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
        onIsLogin: {
            console.log(status)
            if(status===true){
                login.enabled = false
                logout.enabled = true
                logout.text = qsTr("Logout as " + (user ? user.email : ""))
            }
            else{
                login.enabled = true
                logout.enabled = false
                logout.text = qsTr("Logout")
            }
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
                    item_qml.connectSN("omniedge","66YRd88kyYdhzk")
                }
            }
            MenuItem {
                id:disconnectMenu
                text: qsTr("Disconnect")
                enabled: false
                onTriggered: {
                    connectMenu.enabled = true
                    disconnectMenu.enabled = false
                    item_qml.disconnectSN()
                }
            }
            MenuSeparator{}
            MenuItem {
                text: qsTr("Dashboard")
                onTriggered: Qt.openUrlExternally("http://dashboard.omniedge.io")
            }
            MenuSeparator{}
            Menu {
                title: "Network Devices"
                visible: true
                id: contextMenu
                Instantiator {
                    model: vns
                    MenuItem {
                       id:deviceMenu
                       text: modelData.ipPrefix
                    }
                    // The trick is on those two lines
                    onObjectAdded: contextMenu.insertItem(index, object)
                    onObjectRemoved: contextMenu.removeItem(object)
                }
            }
            MenuSeparator{}
            MenuItem {
                id:login
                text: qsTr("Log in...")
                onTriggered:{
                    login.enabled = false
                    logout.enabled = true
                    logout.text = qsTr("Logout as test6@gmail.com")
                    item_qml.oauth()
                }
            }
            MenuItem {
                id:logout
                text: qsTr("Logout")
                onTriggered:
                {
                    login.enabled = true
                    logout.enabled = false
                    logout.text = qsTr("Logout")
                    contextMenu.removeItem(contextMenu.items[0])
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
