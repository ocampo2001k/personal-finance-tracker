import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import App.Models 1.0

ApplicationWindow {
    id: root
    visible: true
    width: 400
    height: 600
    title: qsTr("Finance Tracker")

    TransactionModel {
        id: txModel
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 8

        Button {
            text: qsTr("Add Dummy Transaction")
            onClicked: {
                txModel.addTransaction(1, 12.34, new Date(), "Coffee", "Food")
            }
        }

        ListView {
            id: list
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: txModel
            delegate: Item {
                width: list.width
                height: 50
                Row {
                    spacing: 8
                    Text { text: model.date.toLocaleDateString() }
                    Text { text: model.description }
                    Rectangle { color: "transparent"; width: 1 }
                    Text { text: "$" + model.amount }
                }
            }
            clip: true
        }
    }
} 