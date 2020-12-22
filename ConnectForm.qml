import QtQuick 2.4
import QtQuick.Controls 2.3

Page {
    id: connectPage
    objectName: "item_connect"
    width: 300
    height: 540
    signal connetToSN(string server_ip, string port, string community_name, string password, string internal_ip)
    signal statusOfConnect(int status);
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

        TextField {
            id: serverIP
            width: 150
            height: 40
            text: "52.80.139.238"
            anchors.verticalCenterOffset: 0
            anchors.horizontalCenterOffset: -50
            placeholderText: "Server IP"
            horizontalAlignment: Text.AlignLeft
            font.family: "Arial"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: 2
            font.pointSize: 12
            focus: true
            selectByMouse:true

        }

        TextField {
            id: serverPort
            width: 80
            height: 40
            text: "7787"
            anchors.verticalCenterOffset: 0
            font.family: "Arial"
            anchors.horizontalCenterOffset: 80
            placeholderText: "Port"
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.pointSize: 12
            focus: true
            anchors.margins: 2
            selectByMouse:true
        }

        TextField {
            id: tapIP
            x: 27
            y: 312
            width: 246
            height: 40
            text: "10.254.1.2"
            font.family: "Arial"
            font.pointSize: 12
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 100
            placeholderText: qsTr("TAP IP")
            anchors.horizontalCenter: parent.horizontalCenter
            selectByMouse:true
        }

        TextField {
            id: serverName
            width: 246
            height: 40
            text: "omniedge"
            font.family: "Arial"
            font.pointSize: 12
            placeholderText: qsTr("Server name")
            anchors.verticalCenterOffset: 50
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            selectByMouse:true
        }

        TextField {
            id: password
            width: 246
            height: 40
            text: "66YRd88kyYdhzk"
            font.family: "Arial"
            font.pointSize: 12
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 150
            placeholderText: qsTr("Password")
            anchors.horizontalCenter: parent.horizontalCenter
            echoMode: TextInput.PasswordEchoOnEdit
            selectByMouse:true
        }


        Button {
            id: buttonConnect
            x: 30
            y: 463
            width: 243
            height: 40
            text: qsTr("Connect")
            flat: false
            antialiasing: false
            highlighted: true
            anchors.verticalCenterOffset: 220
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            onClicked: {
                buttonConnect.text = qsTr("Connecting")
                connetToSN(serverIP.text,serverPort.text,serverName.text,password.text,tapIP.text)
            }
        }
    onStatusOfConnect: {
        if(status===1)  //success
        {
            connectForm.visible = false
            statusForm.visible = true
            buttonConnect.text = qsTr("Disconnect")
        }
        else    //fail or timeout
        {
            connectForm.visible = true
            statusForm.visible = false
            buttonConnect.text = qsTr("Connect")
        }
    }
}

