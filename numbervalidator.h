#ifndef NUMBERVALIDATOR_H
#define NUMBERVALIDATOR_H

#include "number.h"

class NumberValidator
{
public:
    NumberValidator();
    bool validateNumber(Number number);
    bool testIfValidForMultiplicationOrDivision(QString numberString);
private:
    bool testIfNumberIsInBase(QString numberString, int base);
};

#endif // NUMBERVALIDATOR_H
