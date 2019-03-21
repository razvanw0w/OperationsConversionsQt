#ifndef DIVISIONRESULT_H
#define DIVISIONRESULT_H

#include <QString>

class DivisionResult
{
public:
    DivisionResult(QString quotient = "", QString remainder = "");
    QString getQuotient();
    QString getRemainder();
private:
    QString quotient;
    QString remainder;
};

#endif // DIVISIONRESULT_H
