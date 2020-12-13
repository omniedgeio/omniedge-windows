import QtQuick 2.0
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
Page {
        id: statusPage
        width: 300
        height: 540

        TabView {
            id: tabView
            width: 276
            height: 474
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            style: TabViewStyle {
                frameOverlap: 1
                tab: Rectangle {
                    color:"grey"
                    implicitWidth: Math.min(txtTitle.width+10, 100)
                    implicitHeight: 40
                    radius: 2
                    Text {
                        id: txtTitle
                        anchors.centerIn: parent
                        text: styleData.title
                        color: "white"
                        font.pointSize: 12
                    }
                }
            }
            Tab {
                id: tab_list
                title: qsTr("设备列表")
                TableView {
                    id: tapIPTable
                    model: ListModel { id: tapIPModel }
                    x: 8
                    y: 0
                    anchors.fill: parent
                    frameVisible: false
                    itemDelegate: Item {
                        Text {
                            color: "Black"
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            elide: styleData.elideMode
                            text: styleData.value
                            font.pointSize: 12
                        }
                    }
                    headerDelegate: Rectangle {
                        height: 40
                        color: "grey"
                        Text {
                            id: headerName
                            text: styleData.value
                            font.pointSize: 12
                            color: "white"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            anchors.fill: parent
                        }
                    }
                    rowDelegate: Rectangle {
                        id: rowRectangle
                        property color rowColor:(styleData.alternate ? "WhiteSmoke" : "Gainsboro")
                        color: rowColor
                        height: 40
                    }
//                    style: TableViewStyle {
//                        handle: Rectangle {
//                            color: "#222222"
//                            implicitWidth: 15
//                        }
//                        scrollBarBackground: Rectangle {
//                            color: "#555555"
//                            implicitWidth: 15
//                        }
//                        incrementControl: Item {
//                        }
//                        decrementControl: Item {
//                        }
//                    }
                    TableViewColumn {
                        role: "id"
                        title: qsTr("No.")
                        width:36
                        movable: false
                    }
                    TableViewColumn {
                        role: "device"
                        title: qsTr("Device")
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
                        title: qsTr("Ping")
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
}
