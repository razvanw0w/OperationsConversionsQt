#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "numbervalidator.h"

class Controller
{
public:
    Controller();
    QString convertNumber(QString initialNumberString, int sourceBase, int destinationBase, bool intermediaryBase10);
    QString rapidlyConvertNumber(QString initialNumberString, int sourceBase, int destinationBase);
    QString executeArithmeticOperation(QString firstNumberString, QString secondNumberString, int base, QChar operation);

private:
    NumberValidator numberValidator;
    QString convertNumberBySubstitution(Number number, int destinationBase);
    QString convertNumberBySuccessiveDivisions(Number number, int destinationBase);
    QString convertNumberByPassingThroughBase10(Number number, int destinationBase);
    QString rapidlyConvertNumberIntoBase2(Number number);
    QString rapidlyConvertBase2NumberIntoDifferentBase(Number number, int destinationBase);
    QString convertPowerOf2BaseDigitIntoBinary(QChar digit, int base);
    void removeLeadingZeros(QString& numberString);
    int getDigitFromCharacter(QChar character);
    QChar getCharacterFromDigit(int digit);
    char digitLookupTable[17] = "0123456789ABCDEF";
};

#endif // CONTROLLER_H
