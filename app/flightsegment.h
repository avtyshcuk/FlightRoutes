#ifndef FLIGHTSEGMENT_H
#define FLIGHTSEGMENT_H

#include <QLineF>

#include "manoeuvre.h"

class FlightSegment
{
public:
    FlightSegment(const QLineF &line,
                  const Manoeuvre &manoeuvre = Manoeuvre());

    QLineF line() const { return mLine; }
    Manoeuvre manoeuvre() const { return mManoeuvre; }
    bool isValid() const { return mManoeuvre.isNull() || mManoeuvre.isValid(); }
    bool isVirtual() const { return mIsVirtual; }
    void setIsVirtual(bool isVirtual) { mIsVirtual = isVirtual; }

private:
    QLineF mLine;
    Manoeuvre mManoeuvre;
    bool mIsVirtual = false;
};

#endif // FLIGHTSEGMENT_H
