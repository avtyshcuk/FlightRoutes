import QtQuick 2.9
import QtLocation 5.9

import "GeoRoutes.js" as GeoRoutes

MapQuickItem {
    id: root

    property int id: -1
    property int index: -1

    anchorPoint.x: referencePoint.width / 2
    anchorPoint.y: referencePoint.height / 2
    sourceItem: Rectangle {
        id: referencePoint
        readonly property int normalSize: 10
        readonly property int editModeSize: 20
        width: normalSize
        height: width
        radius: width / 2
        color: 'red'
        border.color: 'black'

        MouseArea {
            id: pointMouseArea
            anchors.fill: parent
            hoverEnabled: true

            onEntered: {
                if (!flightPlayer.isStopped || flightRegistry.hasActiveFlight) {
                    return;
                }

                referencePoint.width = referencePoint.editModeSize;
                if (!flightRegistry.hasActiveFlight) {
                    GeoRoutes.createPixelRoute(id, index);
                    referencePoint.width = referencePoint.normalSize;
                }
            }

            onExited: {
                if (!flightPlayer.isStopped || !flightRegistry.isBeingModified) {
                    return;
                }

                flightRegistry.finalizeFlightUpdate(index, Qt.point(x, y));
            }
        }
    }
}

