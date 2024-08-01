import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

GridLayout {
    Layout.alignment: Qt.AlignTop
    Layout.margins: 10

    columns: (window.width > 700) ? 1 : 4

    AppButton {
        onClicked: fileDialog.open()
        text: qsTr("Open")
    }
    AppButton {
        onClicked: wordCounterController.start()
        enabled: wordCounterController.isCanceled || wordCounterController.isPaused
        text: qsTr("Start")
    }
    AppButton {
        onClicked: wordCounterController.pause()
        enabled: !wordCounterController.isCanceled && !wordCounterController.isPaused
        text: qsTr("Pause")
    }
    AppButton {
        onClicked: wordCounterController.abort()
        enabled: !wordCounterController.isCanceled
        text: qsTr("Abort")
    }
}

