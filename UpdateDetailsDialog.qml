import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.XmlListModel 2.0

Dialog {
  id: root
  width: 450
  height: 250
  title: qsTr("Update Detail")
  modality: Qt.ApplicationModal
  visible: false
  property alias xml: updateDetailModel.xml
  standardButtons: StandardButton.Yes | StandardButton.No

  XmlListModel {
    id: updateDetailModel
    query: "/updates/update"
    XmlRole { name: "name"; query: "@name/string()" }
    XmlRole { name: "version"; query: "@version/string()" }
    XmlRole { name: "size"; query: "@size/string()" }
  }
  ColumnLayout {
    anchors.left: parent.left
    anchors.right: parent.right
    spacing: 5
    Text {
      text: qsTr("The following update was found. Do you want to update?")
    }
    TableView {
      Layout.fillWidth: true
      Layout.fillHeight: true
      TableViewColumn{ role: "name" ; title: "Name"; width: 200 }
      TableViewColumn{ role: "version"; title: "Version"; width: 80 }
      TableViewColumn{ role: "size"; title: "Size"; width: 80 }
      model: updateDetailModel
    }
  }
}
