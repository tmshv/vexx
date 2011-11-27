import QtQuick 1.0

Rectangle {
    id: property
    property alias text: label.text
    width: childrenRect.width
    height: childrenRect.height

    Text {
        id: label
    }
}
