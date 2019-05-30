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
                id: addStopButton
                text: internal.addStopIcon
                font.family: myCustomFont.name
                onClicked: {
                    if (!flightPlayer.isStopped) {
                        flightPlayer.isStopped = true;
                        return;
                    }

                    if (!flightRegistry.hasActiveFlight) {
                        flightRegistry.resetActiveFlight();
                        var radius = GeoRoutes.calculateRadius();
                        flightRegistry.setCurrentPixelRadius(radius);
                    } else {
                        flightRegistry.finalizeActiveFlight();
                    }
                }
            }

            ToolButton {
                id: playButton
                text: internal.playPauseIcon
                font.family: myCustomFont.name
                visible: !internal.hasActiveFlight
                onClicked: {
                    flightPlayer.isPlaying = !flightPlayer.isPlaying;
                }
            }
        }
    }

    QtObject {
        id: internal

        readonly property bool hasActiveFlight: flightRegistry.hasActiveFlight &&
                                                !flightRegistry.isBeingModified
        readonly property string addStopIcon: !flightPlayer.isStopped ?
                                                  "\uE802" : hasActiveFlight ?
                                                      "\uE808" : "\uE807"
        readonly property string playPauseIcon: flightPlayer.isPlaying ? "\uE803" : "\uE801"
    }
}
