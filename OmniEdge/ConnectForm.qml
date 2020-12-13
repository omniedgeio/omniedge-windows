import QtQuick 2.4
import QtQuick.Controls 2.3

Page {
    id: connectPage
    width: 450*2/3
    height: 810*2/3
    Image {
        width: 120
        height: 120
        anchors.verticalCenterOffset: -180
        anchors.horizontalCenterOffset: 0
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        source: "./images/logo.jpeg"
    }
    Text {
        id: textAppName
        text: qsTr("OmniEdge")
        anchors.verticalCenterOffset: -100
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        renderType: Text.NativeRendering
        font.pixelSize: 24
        font.bold: true
        font.family: "Segoe UI"
    }

    Text {
        id: textSlogan
        text: qsTr("Connect without concern")
        anchors.verticalCenterOffset: -70
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 16
        font.family: "Segoe UI"
    }

    Column {
        id: gridNet
        x: 19
        y: 226
        width: 263
        height: 220

        TextField {
            id: serverIP
            width: 150
            height: 40
            anchors.verticalCenterOffset: -70
            anchors.horizontalCenterOffset: -50
            placeholderText: "Server IP"
            selectionColor: "#800000"
            horizontalAlignment: Text.AlignLeft
            font.family: "Arial"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: 2
            font.pointSize: 12
            focus: true
        }

        TextField {
            id: serverPort
            width: 80
            height: 40
            anchors.verticalCenterOffset: -70
            font.family: "Arial"
            anchors.horizontalCenterOffset: 80
            placeholderText: "Port"
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.pointSize: 12
            focus: true
            anchors.margins: 2
        }

        TextField {
            id: tapIP
            x: 27
            y: 312
            width: 246
            height: 40
            font.family: "Arial"
            font.pointSize: 12
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 30
            placeholderText: qsTr("TAP IP")
            anchors.horizontalCenter: parent.horizontalCenter
        }

        TextField {
            id: serverName
            width: 246
            height: 40
            font.family: "Arial"
            font.pointSize: 12
            placeholderText: qsTr("Server name")
            anchors.verticalCenterOffset: -20
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }

        TextField {
            id: password
            width: 246
            height: 40
            font.family: "Arial"
            font.pointSize: 12
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 80
            placeholderText: qsTr("Password")
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    Button {
        id: buttonConnect
        x: 30
        y: 463
        width: 243
        height: 40
        text: qsTr("Connect")
        autoRepeat: false
        flat: false
        antialiasing: false
        highlighted: true
        anchors.verticalCenterOffset: 220
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        rotation: 0
    }

}

