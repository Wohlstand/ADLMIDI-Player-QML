import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.1

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    FileDialog {
        id: fileDialog
        modality: Qt.WindowModal
        title: "Choose a file"
        selectExisting: true
        selectMultiple: false
        selectFolder: false
        folder: "file://sdcard/Music/midi"
        nameFilters: [ "Music files (*.mid *.rmi *.kar)", "All files (*)" ]
        selectedNameFilter: "Music files (*.mid *.rmi *.kar)"
        sidebarVisible: false
        onAccepted: {
            console.log("Accepted: " + fileUrls)
            for (var i = 0; i < fileUrls.length; ++i)
                midiPlayer.open(fileUrls[i])
        }
        onRejected: { console.log("Rejected") }
    }

    MainForm {
        quitapp.onClicked: {
            midiPlayer.stop()
            Qt.quit()
        }
        play.onClicked: {
            midiPlayer.playpause()
        }
        open.onClicked: {
            fileDialog.open()
        }
        anchors.fill: parent
    }
}
