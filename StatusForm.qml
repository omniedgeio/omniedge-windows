import QtQuick 2.0
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
Page {
        id: statusPage
        objectName: "item_status"
        width: 300
        height: 540
        signal statusOfPing(string ip,int delay)
        TabView {
            id: tabView
            width: 276
            height: 474
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            style: TabViewStyle {
                frameOverlap: 1
                tab: Rectangle {
                    color:"white"
                    implicitWidth: Math.min(txtTitle.width+10, 100)
                    implicitHeight: 38
                    radius: 2
                    Text {
                        id: txtTitle
                        anchors.centerIn: parent
                        text: styleData.title
                        color: "#111827"
                        font.pointSize: 12
                    }
                }
            }
            Tab {
                id: tab_list
                title: qsTr("Device list")
                TableView {
                    id: tapIPTable
                    model: tapIPModel
                    x: 8
                    y: 0
                    anchors.fill: parent
                    frameVisible: false
                    itemDelegate: Item {
                        Text {
                            color: "#111827"
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            elide: styleData.elideMode
                            text: styleData.value
                            font.pointSize: 10
                            font.weight: Font.Light
                        }
                    }
                    headerDelegate: Rectangle {
                        height: 40
                        clip: true
                        anchors.centerIn: parent
                        Rectangle {
                            anchors.fill: parent
                            anchors.rightMargin: -border.width
                            anchors.topMargin:  -border.width
                            anchors.leftMargin: -border.width
                            border.width: 1
                            border.color:"#9CA3AF"
                        }
                        Text {
                            id: headerName
                            text: styleData.value
                            font.pointSize: 8
                            font.weight: Font.Bold
                            color: "#9CA3AF"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            anchors.fill: parent
                        }
                    }
                    rowDelegate: Rectangle {
                        id: rowRectangle
                        //property color rowColor:(styleData.alternate ? "WhiteSmoke" : "Gainsboro")
                        color: "white"
                        height: 40
                    }
                    TableViewColumn {
                        role: "id"
                        title: qsTr("NO.")
                        width:36
                        movable: false
                    }
                    TableViewColumn {
                        role: "device"
                        title: qsTr("DEVICE")
                        width:100
                        movable: false
                    }
                    TableViewColumn {
                        role: "ip"
                        title: qsTr("IP")
                        width:100
                        movable: false
                    }
                    TableViewColumn {
                        role: "ping"
                        title: qsTr("PING")
                        width:40
                        movable: false
                    }
                    Component.onCompleted: {
                        tapIPModel.append({id:"1",device:"Git Server",ip:"10.254.1.1",ping:"10ms"})
                        tapIPModel.append({id:"2",device:"Data Server",ip:"10.254.1.2",ping:"10ms"})
                        tapIPModel.append({id:"3",device:"ERP Server",ip:"10.254.1.3",ping:"10ms"})
                        tapIPModel.append({id:"4",device:"Macbook Pro",ip:"10.254.1.4",ping:"10ms"})
                        tapIPModel.append({id:"5",device:"iphone",ip:"10.254.1.5",ping:"10ms"})
                        tapIPModel.append({id:"6",device:"NAS",ip:"10.254.1.6",ping:"10ms"})
                        tapIPModel.append({id:"7",device:"Git Server",ip:"10.254.1.7",ping:"10ms"})
                        tapIPModel.append({id:"8",device:"iphone",ip:"10.254.1.8",ping:"10ms"})
                        tapIPModel.append({id:"9",device:"Android phone",ip:"10.254.1.9",ping:"10ms"})
                    }
                }
            }
        }
        ListModel { id: tapIPModel }
        onStatusOfPing: {
           if(delay>1)
                tapIPModel.get(ip.split(".")[3] - 1).ping = delay.toString()+"ms"
            else
                tapIPModel.get(ip.split(".")[3] - 1).ping = delay.toString()
        }

}
