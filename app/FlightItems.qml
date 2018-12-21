import QtQuick 2.9

import Flight 1.0

Repeater {
    id: trajectoryView
    model: flightRegistry.hasActiveFlight ?
               flightRegistry.flightModel : []

    FlightItem {
        anchors.fill: parent
        startPoint: start
        endPoint: end
        manoeuvreRect: rect
        manoeuvreStartAngle: startAngle
        manoeuvreSpanAngle: spanAngle
        isVirtualLink: isVirtual
    }
}
