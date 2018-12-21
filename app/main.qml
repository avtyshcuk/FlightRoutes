import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtLocation 5.9
import QtPositioning 5.6

import "GeoRoutes.js" as GeoRoutes

ApplicationWindow {
    id: root
    visible: true
    width: 500
    height: width
    title: qsTr("Flight Routes")

    header: AppToolBar { }

    Map {
        id: map
        anchors.fill: parent
        plugin: Plugin { name: "osm" }
        center: QtPositioning.coordinate(50.45, 30.52)
        gesture.enabled: !flightRegistry.hasActiveFlight
        zoomLevel: 7

        FlightItems { id: flightItems }
        FlightPoints { id: flightPoint }
        MapMouseArea { id: mapMouseArea }

        Connections {
            target: flightRegistry

            onHasActiveFlightChanged: {
                if (!flightRegistry.hasActiveFlight) {
                    GeoRoutes.addGeoRoute(map);
                    GeoRoutes.addGeoPoints(map);
                }
            }
        }
    }
}
