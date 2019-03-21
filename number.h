#ifndef NUMBER_H
#define NUMBER_H

#include <QString>
#include "divisionresult.h"

class Number
{
public:
    Number();
    Number(QString, int);
    QString getNumber();
    int getBase();
    Number operator+ (Number const& otherNumber);
    Number operator- (Number const& otherNumber);
    Number operator* (Number const& otherNumber);
    DivisionResult operator/ (Number const& otherNumber);
    Number operator= (Number const& otherNumber);
    bool operator< (Number const& otherNumber);


private:
    QString number;
    int base;
    const char digitLookupTable[17] = "0123456789ABCDEF";
    void makeNumberStringsUniform(QString& firstString, QString& secondString, int &firstStringSize, int &secondStringSize);
    int getDigitFromCharacter(QChar character);
    QChar getCharacterFromDigit(int digit);
};

#endif // NUMBER_H
