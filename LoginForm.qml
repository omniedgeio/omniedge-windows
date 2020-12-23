import QtQuick 2.0
import QtQuick.Controls 2.3

Page {
    id: loginPage
    width: 300
    height: 540
    Image {
        x: 165
        y: 189
        width: 120
        height: 120
        anchors.verticalCenterOffset: -150
        anchors.horizontalCenterOffset: 0
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        source: "./images/logo.jpeg"
    }

    Item{
        id:item_auth
        objectName: "item_auth"
        signal googleOAuth()
        anchors.verticalCenterOffset: 80
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        Button {
            id:googleButton
            width: 180
            text: qsTr("Sign in with Google")
            flat: true
            icon.source : "qrc:/images/google.png"
            icon.color: "transparent"
            font.family: "Segoe UI"
            background:Rectangle {
                border.width: googleButton.pressed?2:1;
                border.color: "gray"
            }
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            onClicked: {
                item_auth.googleOAuth()
                //connectForm.visible = true
                //loginForm.visible = false
            }
        }
    }
    Text {
        id: textAppName
        text: qsTr("OmniEdge")
        anchors.verticalCenterOffset: -60
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
        anchors.verticalCenterOffset: -30
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 16
        font.family: "Segoe UI"
    }
}
