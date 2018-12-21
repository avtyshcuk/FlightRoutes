import QtQuick 2.9

Repeater {
    id: pointsView
    model: flightRegistry.hasActiveFlight ?
               flightRegistry.flightModel.points : []

    Rectangle {
        x: modelData.x - radius
        y: modelData.y - radius
        radius: 5
        width: 2*radius
        height: width
        color: 'red'
        border.color: 'black'
    }
}
