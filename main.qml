import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Dialogs 1.2
import com.example.plugin.maintenancetool 1.0

ApplicationWindow {
  visible: true
  width: 640
  height: 480
  title: qsTr("Hello World")

  MaintenanceTool {
    id: maintenancetool
    property bool automatic: false

    onStateChanged: {
      if(state === MaintenanceTool.NotRunning){
        if(hasUpdate){
          console.debug("detail:" + updateDetails)
          updateDetailDlg.xml = updateDetails
          updateDetailDlg.open()
        }else if(!automatic){
          notFoundDlg.open()
        }
      }
    }
  }

  Timer {
    interval: 2000
    repeat: false
    running: true
    onTriggered: {
      maintenancetool.automatic = true
      maintenancetool.checkUpdate()
    }
  }

  //メニュー
  menuBar: MenuBar {
    Menu {
      title: qsTr("&File")
      MenuItem {
        text: qsTr("&Check update")
        onTriggered: {
          maintenancetool.automatic = false
          maintenancetool.checkUpdate()
        }
      }
      MenuItem {
        text: qsTr("&Exit")
        onTriggered: Qt.quit()
      }
    }
  }

  Text {
    text: qsTr("Hello World")
    anchors.centerIn: parent
  }

  UpdateDetailsDialog {
    id: updateDetailDlg
    onYes: {
      maintenancetool.startMaintenanceTool()   //メンテツール起動
      if(Qt.platform.os == "windows"){
        Qt.quit()
      }
    }
    onNo: { console.debug("No thank you") }
  }
  MessageDialog {
    id: notFoundDlg
    title: qsTr("Notification")
    text: qsTr("Not found update.")
    standardButtons: StandardButton.Ok
  }
}
