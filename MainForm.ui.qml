import QtQuick 2.7
import QtQuick.Controls 2.0

Rectangle {
    id: rectangle1

    width: 360
    height: 600
    color: "#000000"
    property alias quitapp: quitapp
    property alias play: play
    property alias open: open

    Button {
        id: open
        x: 130
        width: 224
        height: 84
        text: qsTr("Открыть")
        anchors.top: parent.top
        anchors.topMargin: 8
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Button {
        id: play
        x: 130
        width: 224
        height: 84
        text: qsTr("Сыграть")
        anchors.top: parent.top
        anchors.topMargin: 104
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Button {
        id: quitapp
        x: 68
        y: 454
        width: 224
        height: 84
        text: qsTr("Quit")
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 62
        transformOrigin: Item.Bottom
    }



}
