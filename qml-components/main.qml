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

    footer: ProgressBar {
        id: progressBar

        indeterminate: true
        visible: true
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
