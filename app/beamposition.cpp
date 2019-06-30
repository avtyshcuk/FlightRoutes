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
            auto dataString = QString::fromUtf8(datagram.data());
            auto dataList = dataString.split(',');
            auto samLatitude = dataList.at(0).toDouble();
            auto samLongitude = dataList.at(1).toDouble();

            mSamPosition = QGeoCoordinate(samLatitude, samLongitude);
            mSamBeamRange = dataList.at(2).toDouble();
            mSamBeamAngle = dataList.at(3).toDouble();

            emit beamPositionUpdated();
        }
    });
}
