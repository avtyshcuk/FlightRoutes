function addGeoPoints(map) {
    var points = flightRegistry.activeGeoPoints();
    for (var i = 0; i < points.length; ++i) {
        var coordinate = points[i];
        var params = {"coordinate": coordinate};
        var component = Qt.createComponent("GeoPoint.qml");
        var geoPoint = component.createObject(map, params);
        map.addMapItem(geoPoint);
    }
}

function addGeoRoute(map) {
    var path = flightRegistry.activeGeoPath();
    var pathComponent = Qt.createComponent("GeoPath.qml");
    var geoPath = pathComponent.createObject(map, {"path": path});
    map.addMapItem(geoPath);
}

function calculateRadius() {

    // We have to calculate how many pixels
    // is our 'boeing-747' manoeuvre radius
    // check "geoflighroute.cpp" for details
    var mapCenter = map.center;
    var radiusPoint = mapCenter.atDistanceAndAzimuth(21770, 0);
    var centerPixel = map.fromCoordinate(mapCenter);
    var pointPixel = map.fromCoordinate(radiusPoint);
    var radiusPixel = centerPixel.y - pointPixel.y;

    return radiusPixel;
}
