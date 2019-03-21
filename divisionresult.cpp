#include "divisionresult.h"

DivisionResult::DivisionResult(QString quotient, QString remainder): quotient{quotient}, remainder{remainder}
{}

QString DivisionResult::getQuotient() {
    return quotient;
}

QString DivisionResult::getRemainder() {
    return remainder;
}
