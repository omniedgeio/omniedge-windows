import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Window {
    id: window
    visible: true
    width: 450*2/3
    height: 810*2/3
    title: qsTr("OmniEdge")

    LoginForm{
        id:loginForm
        visible: true
    }
    ConnectForm{
        id:connectForm
        visible: false
    }

}
