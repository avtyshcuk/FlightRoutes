#ifndef FLIGHTSEGMENT_H
#define FLIGHTSEGMENT_H

#include <QLineF>

#include "manoeuvre.h"

class FlightSegment
{
public:
    FlightSegment(const QLineF &line, bool isVirtual = false,
                  const Manoeuvre &manoeuvre = Manoeuvre());

    QLineF line() const { return mLine; }
    Manoeuvre manoeuvre() const { return mManoeuvre; }
    bool isValid() const { return mManoeuvre.isNull() || mManoeuvre.isValid(); }
    bool isVirtual() const { return mIsVirtual; }
    void setIsVirtual(bool isVirtual) { mIsVirtual = isVirtual; }

private:
    QLineF mLine;
    bool mIsVirtual;
    Manoeuvre mManoeuvre;
};

#endif // FLIGHTSEGMENT_H
