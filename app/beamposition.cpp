#include "beamposition.h"

#include <QDebug>
#include <QNetworkDatagram>
#include <QGeoCoordinate>

#include "flightplayer.h"

BeamPosition::BeamPosition(QObject *parent)
    : QUdpSocket(parent)
{
    bind(45454, QUdpSocket::ShareAddress);
    QObject::connect(this, &QUdpSocket::readyRead, [this]{
        while (hasPendingDatagrams()) {

            auto datagram = receiveDatagram();
            auto data = QString::fromUtf8(datagram.data());
            auto beamParameters = data.split(',');

            auto latitude = beamParameters.at(0).toDouble();
            auto longitude = beamParameters.at(1).toDouble();
            auto range = beamParameters.at(2).toDouble();
            auto angle = beamParameters.at(3).toDouble();

            emit beamParametersUpdated(QGeoCoordinate(latitude, longitude),
                                       range, angle);
        }
    });
}
