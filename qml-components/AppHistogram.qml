import QtQuick
import QtQuick.Layouts

RowLayout {
    id: rootLayout

    Layout.fillHeight: true
    Layout.fillWidth: true
    Layout.preferredHeight: 300
    Layout.margins: 10
    Layout.alignment: Qt.AlignBottom

    property real maxValue: Math.max.apply(Math, wordCounterController.histogram.map(item => item.value))

    Repeater {
        model: wordCounterController.histogram
        delegate: ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredWidth: 20
            Layout.alignment: Qt.AlignBottom

            Text {
                id: valueText

                Layout.fillWidth: true

                wrapMode: Text.NoWrap
                text: modelData.value
                horizontalAlignment: Text.AlignHCenter
                elide: Text.ElideRight
            }
            Rectangle {
                Layout.fillWidth: true

                implicitHeight: modelData.value * (rootLayout.height - valueText.height - labelText.height) / maxValue
                color: "blue"
            }
            Text {
                id: labelText

                Layout.fillWidth: true

                wrapMode: Text.NoWrap
                text: modelData.key
                horizontalAlignment: Text.AlignHCenter
                elide: Text.ElideRight
            }
        }

        onItemAdded: function(index, item) {
            if ((index + 1) > 15)
                item.visible = false
        }
    }
}
