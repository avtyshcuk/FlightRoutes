#include "flightsegment.h"

FlightSegment::FlightSegment(const QLineF &line, bool isVirtual,
                             const Manoeuvre &manoeuvre)
    : mLine(line)
    , mIsVirtual(isVirtual)
    , mManoeuvre(manoeuvre)
{

}
