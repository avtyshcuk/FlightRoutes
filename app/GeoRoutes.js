var routesRegistry = {};
var flightPointsRegistry = [];

function registerRoute(map) {
    var id = flightRegistry.activeFlightId;
    if (id == -1) {
        return;
    }

    routesRegistry[id] = [];

    addGeoRoute(map);
    addGeoPoints(map);
}

function addGeoPoints(map) {
    var points = flightRegistry.activeGeoPoints();
    var id = flightRegistry.activeFlightId;
    for (var i = 0; i < points.length; ++i) {
        var coordinate = points[i];
        var params = {
            "coordinate": coordinate,
            "id": id,
            "index": i
        };
        var component = Qt.createComponent("GeoPoint.qml");
        var geoPoint = component.createObject(map, params);
        routesRegistry[id].push(geoPoint);
        map.addMapItem(geoPoint);
    }
}

function addGeoRoute(map) {
    var path = flightRegistry.activeGeoPath();
    var pathComponent = Qt.createComponent("GeoPath.qml");
    var geoPath = pathComponent.createObject(map, {"path": path});
    var id = flightRegistry.activeFlightId;
    routesRegistry[id].push(geoPath);
    map.addMapItem(geoPath);
}

function updateGeoRoute(map) {

    var id = flightRegistry.activeFlightId;
    for (var i = 0; i < routesRegistry[id].length; ++i) {
        map.removeMapItem(routesRegistry[id][i]);
    }
    addGeoRoute(map);
    addGeoPoints(map);
}

function calculateRadius() {

    // We have to calculate how many pixels
    // is our 'boeing-747' manoeuvre radius
    // check "geoflighroute.cpp" for details
    var mapCenter = map.center;
    var radiusPoint = mapCenter.atDistanceAndAzimuth(21770, 0);
    var centerPixel = map.fromCoordinate(mapCenter, false);
    var pointPixel = map.fromCoordinate(radiusPoint, false);
    var radiusPixel = centerPixel.y - pointPixel.y;

    return radiusPixel;
}

function geoToPixelPoints(geoPoints) {
    var points = [];
    for (var i = 0; i < geoPoints.length; ++i) {
        points.push(map.fromCoordinate(geoPoints[i], false));
    }

    return points;
}

function createPixelRoute(id, index) {
    var geoPoints = flightRegistry.flightGeoPoints(id);
    var points = geoToPixelPoints(geoPoints);
    var radius = calculateRadius();

    flightRegistry.prepareFlightUpdate(id, points, radius);
    flightRegistry.setModifiedIndex(index);
}

function updateFlightPoints() {
    for (var i = 0; i < flightPointsRegistry.length; ++i) {
        map.removeMapItem(flightPointsRegistry[i]);
    }
    flightPointsRegistry = [];

    var points = flightPlayer.points();
    var azimuths = flightPlayer.azimuths();
    for (var j = 0; j < points.length; ++j) {
        var coordinate = points[j];
        var azimuth = azimuths[j];
        var params = {
            "coordinate": coordinate,
            "azimuth": azimuth
        };
        var component = Qt.createComponent("GeoAirPlane.qml");
        var geoFlightPoint = component.createObject(map, params);
        flightPointsRegistry.push(geoFlightPoint)
        map.addMapItem(geoFlightPoint);
    }
}
