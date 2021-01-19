import QtQuick 2.11
import QtQuick.Controls 2.3
import Qt.labs.platform 1.1
import GraphQlClient 1.0


ApplicationWindow {
    id: window
    property var subscriptions: [];
    Item {
        id: item_qml
        objectName: "item_qml"
        signal connectSN(string community,string key )
        signal disconnectSN()
        signal oauth()
        signal showMsg(string title,string msg)
        onShowMsg:{
            console.debug("onShowMsg")
            trayIcon.showMessage(title,msg)
        }
    }

    SystemTrayIcon {
        id:trayIcon
        visible: true
        iconSource: "qrc:/images/AppIcon.png"
        tooltip: "OmniEdge: Connect without corcern"
        Component.onCompleted: {
            showMessage("OmniEdge", "Connect without corcern.")
            executeQuery("query {\n  company {\n    ceo\n    coo\n    cto_propulsion\n    cto\n    employees\n    founded\n    founder\n    launch_sites\n    name\n    summary\n    test_sites\n    valuation\n    vehicles\n  }\n}")
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
                onTriggered: Qt.openUrlExternally("http:omniedge.io")
            }
            MenuSeparator{}
            Menu {
                title: "Network Devices"
                MenuItem {
                    text: qsTr("10.254.1.1"+"   ping:30ms")
                }
                MenuItem {
                    text: qsTr("10.254.1.2")
                }
                MenuItem {
                    text: qsTr("10.254.1.3")
                }
            }
            MenuSeparator{}
            MenuItem {
                text: qsTr("Log in...")
                onTriggered:
                    item_qml.oauth()
            }
            MenuItem {
                text: qsTr("Log out")
                //enabled: false
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
                shortcut: "Ctrl+X"
                onTriggered:{
                    Qt.quit()
                    }
                }
            }
    }

    function executeQuery(query) {
            if (query.indexOf("mutation") !== -1) {
                var mutationId = gql.mutate(query);
                console.log("mutation id is: " + mutationId);
            }
            else if (query.indexOf("subscription") !== -1) {
                var subscriptionId = gql.query(query);
                console.log("subscription id is: " + subscriptionId);
                subscriptions.push(subscriptionId);                
            }
            else {
                var queryId = gql.query(query);
                console.log("query id is: " + queryId);
            }
        }

    function unsubscribeAllSubscriptions() {
        console.log(subscriptions.length);
        for(var i = 0; i < subscriptions.length; ++i) {
            var id = subscriptions[i];
            console.log("unsubscribe " + id);
            gql.unsubscribe(id);
        }
        subscriptions = [];
    }

    GraphQlConnection {
        id: gql
        url: "https://api.spacex.land/graphql/"    //using a http connection

        //if you are using a websocket connection use this to open the connection
//        Component.onCompleted: {
//            console.log("completed");
//            open();
//        }

        onDataReceived: {
            //result data is available as json object
            var resultAsJson = JSON.stringify(data, /*replacer*/ null, /*spacing*/ 2);
            console.log(resultAsJson)
            console.log("response from query with id:" + data.id);

            if (data.payload.errors) {
                console.log("having errors");
                console.log(resultAsJson)
            } else {
                console.log(subscriptions.indexOf(data.id));
                if (subscriptions.indexOf(data.id) > 0) {
                    console.log("received a subscription");
                    txtResult.text = "=== Data from subscription: ===\n\n" +  resultAsJson + "\n\n=== End data from subscription";
                } else {
                    console.log(resultAsJson)
                }
            }
        }

        onError: {
            var resultAsJson = JSON.stringify(error, /*replacer*/ null, /*spacing*/ 2);
            console.log(resultAsJson)
            console.log("Error: " + error.message);
        }
    }
}
