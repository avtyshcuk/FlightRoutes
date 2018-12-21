#ifndef FLIGHTMODEL_H
#define FLIGHTMODEL_H

#include <QAbstractListModel>

class Flight;

class FlightModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(bool hasVirtualPart READ hasVirtualPart NOTIFY hasVirtualPartChanged)
    Q_PROPERTY(QVariantList points READ points NOTIFY pointsChanged)

public:
    explicit FlightModel(QObject *parent = nullptr);

    enum FlightRoles {
        StartPoint = 0,
        EndPoint,
        ManoeuvreRect,
        ManoeuvreStartAngle,
        ManoeuvreSpanAngle,
        IsVirtualLink
    };

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setFlight(Flight *flight);
    bool hasVirtualPart() const;
    QVariantList points() const;

signals:
    void hasVirtualPartChanged();
    void pointsChanged();

private:
    QRectF manoeuvreRect(int index) const;
    int manoeuvreStartAngle(int index) const;
    int manoeuvreSpanAngle(int index) const;

private:
    Flight *mFlight;
};

Q_DECLARE_METATYPE(FlightModel*)

#endif // FLIGHTMODEL_H
