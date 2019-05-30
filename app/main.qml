import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtLocation 5.9
import QtPositioning 5.6

ApplicationWindow {
    id: root
    visible: true
    width: 900
    height: width
    title: qsTr("Flight Routes")

    header: AppToolBar { }

    Map {
        id: map
        anchors.fill: parent
        plugin: Plugin { name: "osm" }
        center: QtPositioning.coordinate(50.45, 30.52)
        gesture.enabled: !flightRegistry.hasActiveFlight
        maximumZoomLevel: 10
        minimumZoomLevel: 5
        zoomLevel: 7

        MapMouseArea { id: mapMouseArea }
        FlightItems { id: flightItems }
        FlightPoints { id: flightPoint }
    }
}
