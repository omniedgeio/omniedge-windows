import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("OmniEdge")

    Item{
        id:item_auth
        objectName: "item_auth"
        signal googleAuthClicked()
        Image {
            x: 192
            y: 211
            source: "./images/Sign.png"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                   console.info("image clicked!")
                   item_auth.googleAuthClicked()
                }
            }
        }
    }

}
