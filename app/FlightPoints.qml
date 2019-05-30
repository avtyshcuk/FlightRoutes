import QtQuick 2.9

Repeater {
    id: pointsView
    model: flightRegistry.hasActiveFlight ?
               flightRegistry.flightModel.points : []

    Rectangle {
        id: flightPoint
        x: modelData.x - radius
        y: modelData.y - radius
        z: map.z + 1
        radius: flightRegistry.modifiedIndex === index ? 10 : 5
        width: 2*radius
        height: width
        color: 'red'
        border.color: 'black'

        readonly property point position: Qt.point(x + radius, y + radius)
        onPositionChanged: {
            if (internal.isMoving) {
                internal.updateActiveFlight(index, position);
            }
        }

        MouseArea {
            anchors.fill: parent
            drag.target: parent
            visible: flightRegistry.isBeingModified

            onReleased: {
                var coordinate = map.toCoordinate(position, false);
                flightRegistry.finalizeFlightUpdate(index, coordinate);
                internal.isMoving = false;
            }

            onPressed: {
                internal.isMoving = true;
            }
        }

        QtObject {
            id: internal

            property bool isMoving: false

            function updateActiveFlight(index, point) {
                if (flightRegistry.isBeingModified) {
                    flightRegistry.updateActiveFlight(index, point);
                }
            }
        }
    }
}
