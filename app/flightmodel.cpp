#include <QtMath>

#include "flightmodel.h"
#include "flight.h"

#include <QDebug>

FlightModel::FlightModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

int FlightModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mFlight->size();
}

QVariant FlightModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    switch (role) {
    case FlightRoles::StartPoint:
        return mFlight->flightSegment(index.row()).line().p1();

    case FlightRoles::EndPoint:
        return mFlight->flightSegment(index.row()).line().p2();

    case FlightRoles::ManoeuvreRect:
        return manoeuvreRect(index.row());

    case FlightRoles::ManoeuvreStartAngle:
        return manoeuvreStartAngle(index.row());

    case FlightRoles::ManoeuvreSpanAngle:
        return manoeuvreSpanAngle(index.row());

    case FlightRoles::IsVirtualLink:
        return mFlight->flightSegment(index.row()).isVirtual();

    default:
        return QVariant();
    }
}

QHash<int, QByteArray> FlightModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[FlightRoles::StartPoint] = "start";
    roles[FlightRoles::EndPoint] = "end";
    roles[FlightRoles::ManoeuvreRect] = "rect";
    roles[FlightRoles::ManoeuvreStartAngle] = "startAngle";
    roles[FlightRoles::ManoeuvreSpanAngle] = "spanAngle";
    roles[FlightRoles::IsVirtualLink] = "isVirtual";

    return roles;
}

void FlightModel::setFlight(Flight *flight)
{
    mFlight = flight;

    connect(mFlight, &Flight::pointsChanged, this, &FlightModel::pointsChanged);
    connect(mFlight, &Flight::startFlightUpdate, this, [this](int size){
        emit beginInsertRows(QModelIndex(), size, size);
    });
    connect(mFlight, &Flight::endFlightUpdate, this, &FlightModel::endInsertRows);
    connect(mFlight, &Flight::hasVirtualPartChanged, this, &FlightModel::hasVirtualPartChanged);
    connect(mFlight, &Flight::flightPartUpdate, this, [this](int index) {
        emit dataChanged(createIndex(index, 0), createIndex(index, 0));
    });
}

bool FlightModel::hasVirtualPart() const
{
    return mFlight->hasVirtualPart();
}

QVariantList FlightModel::points() const
{
    return mFlight->points();
}

QRectF FlightModel::manoeuvreRect(int index) const
{
    Manoeuvre manoeuvre = mFlight->flightSegment(index).manoeuvre();
    if (!manoeuvre.isValid()) {
        return QRectF();
    }

    const auto &radius = manoeuvre.radius();
    const auto &center = manoeuvre.circlePoint();

    QPointF topLeft(center.x() - radius, center.y() - radius);
    return QRectF(topLeft, QSizeF(2 * radius, 2 * radius));
}

int FlightModel::manoeuvreStartAngle(int index) const
{
    Manoeuvre manoeuvre = mFlight->flightSegment(index).manoeuvre();
    if (!manoeuvre.isValid()) {
        return 0;
    }

    const auto &startAngle = manoeuvre.startAngle();
    return qRound(qRadiansToDegrees(startAngle));
}

int FlightModel::manoeuvreSpanAngle(int index) const
{
    Manoeuvre manoeuvre = mFlight->flightSegment(index).manoeuvre();
    if (!manoeuvre.isValid()) {
        return 0;
    }

    const auto &spanAngle = manoeuvre.spanAngle();
    const int sign = manoeuvre.isLeftTurn() ? -1 : 1;
    return qRound(qRadiansToDegrees(sign * spanAngle));
}
