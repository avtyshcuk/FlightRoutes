#include "flightairplane.h"

FlightAirPlane::FlightAirPlane(QQuickPaintedItem *parent)
    : QQuickPaintedItem (parent)
{

}

void FlightAirPlane::paint(QPainter *painter)
{
    painter->setBrush(QBrush(Qt::black));
    painter->setRenderHints(QPainter::Antialiasing, true);

    QVector<QPoint> points;
    points << QPoint(10, 1) << QPoint(11, 2) << QPoint(11, 6)
           << QPoint(12, 7) << QPoint(12, 8) << QPoint(20, 12)
           << QPoint(20, 14) << QPoint(12, 12) << QPoint(12, 13)
           << QPoint(11, 14) << QPoint(11, 16) << QPoint(14, 19)
           << QPoint(10, 18) << QPoint(6, 19) << QPoint(6, 18)
           << QPoint(9, 16) << QPoint(9, 14) << QPoint(8, 13)
           << QPoint(8, 12) << QPoint(0, 14) << QPoint(0, 12)
           << QPoint(8, 8) << QPoint(8, 7) << QPoint(9, 6) << QPoint(9, 2);

    painter->drawPolygon(QPolygon(points));
}
