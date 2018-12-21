import QtQuick 2.9
import QtLocation 5.9

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
        flightRegistry.updateActiveFlight(coordinate, point);
    }

    onPositionChanged: {
        if (hoverEnabled) {
            var point = Qt.point(mouseX, mouseY)
            flightRegistry.updateVirtualPart(point);
        }
    }

    QtObject {
        id: internal
        property bool hasVirtual: flightRegistry.hasActiveFlight &&
                                  flightRegistry.flightModel.hasVirtualPart
    }
}
