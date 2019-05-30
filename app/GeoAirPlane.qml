import QtQuick 2.9
import QtLocation 5.9

import Flight 1.0

MapQuickItem {
    id: root

    property real azimuth: 0.0

    anchorPoint.x: referencePoint.width / 2
    anchorPoint.y: referencePoint.height / 2
    sourceItem: FlightAirPlane {
        id: referencePoint
        width: 20
        height: width
        transform: Rotation {
            origin.x: 10
            origin.y: 10
            angle: azimuth
        }
    }
}
