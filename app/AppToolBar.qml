import QtQuick 2.0
import QtQuick.Controls 2.1

import "GeoRoutes.js" as GeoRoutes

ToolBar {
    id: root
    leftPadding: 8

    FontLoader {
        id: myCustomFont
        source: "qrc:/fonts/aerialattackfont.ttf"
    }

    Flow {
        id: flow
        width: parent.width

        Row {
            id: fileRow

            ToolButton {
                id: addButton
                text: internal.addIcon
                font.family: myCustomFont.name
                onClicked: {
                    if (!flightRegistry.hasActiveFlight) {
                        flightRegistry.resetActiveFlight();
                        var radius = GeoRoutes.calculateRadius();
                        flightRegistry.setCurrentPixelRadius(radius);
                    } else {
                        flightRegistry.finalizeActiveFlight();
                    }
                }
            }
        }

    }

    QtObject {
        id: internal
        readonly property string addIcon: flightRegistry.hasActiveFlight &&
                                          !flightRegistry.isBeingModified ? "\uE808" : "\uE807"
    }

}
