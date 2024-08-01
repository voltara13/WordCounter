import QtQuick
import QtQuick.Layouts

GridLayout {
    id: grid

    anchors.fill: parent
    columns: (window.width > 700) ? 2 : 1

    AppHistogram {
    }
    AppButtonPanel {
    }
}
