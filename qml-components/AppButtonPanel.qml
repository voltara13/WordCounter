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
        text: qsTr("Start")
    }
    AppButton {
        text: qsTr("Pause")
    }
    AppButton {
        text: qsTr("Abort")
    }
}

