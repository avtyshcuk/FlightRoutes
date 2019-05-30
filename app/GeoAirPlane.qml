import QtQuick 2.9
import QtLocation 5.9

MapQuickItem {
    id: root

    anchorPoint.x: referencePoint.width / 2
    anchorPoint.y: referencePoint.height / 2
    sourceItem: Rectangle {
        id: referencePoint
        width: 20
        height: width
        radius: width / 2
        color: 'green'
        border.color: 'black'
    }
}
