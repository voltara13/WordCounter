import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import testapp

ApplicationWindow {
    id: window

    height: 600
    width: 800

    title: "Word Frequency Analyzer"
    visible: true

    header: TextArea {
        text: wordCounterController.filePath
        readOnly: true
        wrapMode: TextEdit.WordWrap
    }

    footer: Item {
        anchors.left: parent.left
        anchors.right: parent.right
        height: 20

        ProgressBar {
            id: progressBar

            anchors.fill: parent

            from: 0
            to: 100

            value: wordCounterController.progress
            visible: !wordCounterController.isCanceled
        }
    }

    AppView {
    }

    WordCounterController {
        id: wordCounterController
    }

    FileDialog {
        id: fileDialog
        nameFilters: [ "Text files (*.txt)", "All files (*)" ]
        title: qsTr("Select text file")
        onAccepted: wordCounterController.filePath = selectedFile
    }
}
