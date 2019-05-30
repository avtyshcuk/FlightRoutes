import QtQuick 2.9
import QtLocation 5.9

import "GeoRoutes.js" as GeoRoutes

MouseArea {
    id: mapMouseArea
    anchors.fill: parent
    acceptedButtons: Qt.RightButton | Qt.LeftButton
    hoverEnabled: internal.hasVirtual

    onClicked: {
        if (!flightRegistry.hasActiveFlight) {
            return;
        }

        if (mouse.button & Qt.RightButton) {
            flightRegistry.finalizeActiveFlight();
            return;
        }

        var point = Qt.point(mouseX, mouseY);
        var coordinate = map.toCoordinate(point);
        flightRegistry.addNewPoint(coordinate, point);
    }

    onPositionChanged: {
        if (hoverEnabled) {
            var point = Qt.point(mouseX, mouseY)
            flightRegistry.addVirtualPoint(point);
        }
    }

    Connections {
        target: flightRegistry

        onHasActiveFlightChanged: {
            if (!flightRegistry.hasActiveFlight && !flightRegistry.isBeingModified) {
                GeoRoutes.registerRoute(map);
            }
        }

        onIsBeingModifiedChanged: {
            if (!flightRegistry.isBeingModified) {
                GeoRoutes.updateGeoRoute(map);
            }
        }
    }

    Connections {
        target: flightPlayer

        onFlightPointsUpdated: {
            GeoRoutes.updateFlightPoints();
        }
    }

    QtObject {
        id: internal
        property bool hasVirtual: flightRegistry.hasActiveFlight &&
                                  flightRegistry.flightModel.hasVirtualPart
    }
}
