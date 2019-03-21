#include "number.h"
#include "divisionresult.h"

Number::Number(): number{""}, base{10}
{}

Number::Number(QString externalNumber, int externalBase)
    : number{externalNumber}, base{externalBase}
{}

QString Number::getNumber() {
    return number;
}

int Number::getBase() {
    return base;
}

int Number::getDigitFromCharacter(QChar character) {
    /* This function converts a character to its associated number
     * INPUT: character - QChar
     * OUTPUT: digit - int
     */
    int digit;
    if (character.isLetter())
        digit = character.toLatin1() - 'A' + 10;
    else
        digit = character.toLatin1() - '0';
    return digit;
}

QChar Number::getCharacterFromDigit(int digit) {
    /* This function converts a number to its associated character
     * INPUT: digit - int
     * OUTPUT: character - QChar
     */
    QChar character = digitLookupTable[digit];
    return character;
}

void Number::makeNumberStringsUniform(QString &firstString, QString &secondString, int& firstStringSize, int& secondStringSize) {
    if (firstStringSize != secondStringSize) {
        if (firstStringSize < secondStringSize) {
            QString fillingZeros(secondStringSize - firstStringSize, '0');
            firstString.push_front(fillingZeros);
            firstStringSize = secondStringSize;
        }
        else {
            QString fillingZeros(firstStringSize - secondStringSize, '0');
            secondString.push_front(fillingZeros);
            secondStringSize = firstStringSize;
        }
    }
}

Number Number::operator+(const Number &otherNumber) {
    /* Overloading the + operator, this will help us to add
     * two numbers easier
     * INPUT: otherNumber: Number (represents the second term of the addition)
     * OUTPUT: resultNumber: Number (the result of the addition)
     */
    QString currentNumberString = number;
    QString otherNumberString = otherNumber.number;
    int currentNumberStringSize = currentNumberString.size();
    int otherNumberStringSize = otherNumberString.size();
    // fill with zeroes if needed at the beginning of one of the numbers if needed
    makeNumberStringsUniform(currentNumberString, otherNumberString, currentNumberStringSize, otherNumberStringSize);
    QString resultString = "";
    int carry = 0, firstDigitValue = 0, secondDigitValue = 0;
    int sum, sumRemainder;
    for (int i = currentNumberStringSize - 1; i >= 0; --i) {
        firstDigitValue = getDigitFromCharacter(currentNumberString[i]);
        secondDigitValue = getDigitFromCharacter(otherNumberString[i]);

        sum = firstDigitValue + secondDigitValue + carry;
        sumRemainder = sum % base;
        carry = sum / base;
        resultString.push_front(getCharacterFromDigit(sumRemainder));
    }
    if (carry) {
        resultString.push_front(getCharacterFromDigit(carry));
    }
    return Number(resultString, base);
}

Number Number::operator-(const Number& otherNumber) {
    /* Overloading the - operator, this will help us to subtract two numbers easier
     * INPUT: otherNumber: Number - the subtrahend of the subtraction
     * OUTPUT: resultNumber: Number - the result of the subtraction
     */
    QString currentNumberString = number;
    QString otherNumberString = otherNumber.number;
    int currentNumberStringSize = currentNumberString.size();
    int otherNumberStringSize = otherNumberString.size();
    // fill with zeroes if needed at the beginning of one of the numbers if needed
    makeNumberStringsUniform(currentNumberString, otherNumberString, currentNumberStringSize, otherNumberStringSize);
    QString resultString = "";
    int borrow = 0, firstDigitValue = 0, secondDigitValue = 0;
    int difference = 0;
    for (int i = currentNumberStringSize - 1; i >= 0; --i) {
        firstDigitValue = getDigitFromCharacter(currentNumberString[i]);
        secondDigitValue = getDigitFromCharacter(otherNumberString[i]);
        difference = borrow + firstDigitValue - secondDigitValue;
        borrow = 0;
        if (difference < 0) { // we need to borrow 1 unit from the next digit
            difference += base;
            borrow = -1;
        }
        resultString.push_front(getCharacterFromDigit(difference));
    }
    return Number(resultString, base);
}

Number Number::operator*(const Number &otherNumber) {
    /* Overloading the * operator, this will help us to multiply two numbers easier
     * INPUT: otherNumber: Number - the second term of multiplication
     * OUTPUT: resultNumber: Number - the result of the multiplication
     */
    QString currentNumberString = number, resultString = "";
    int firstDigitValue;
    int secondDigitValue = getDigitFromCharacter(otherNumber.number[0]);
    int currentNumberStringSize = currentNumberString.size();
    int carry = 0, product = 0, productRemainder;
    for (int i = currentNumberStringSize - 1; i >= 0; --i) {
        firstDigitValue = getDigitFromCharacter(currentNumberString[i]);
        product = carry + firstDigitValue * secondDigitValue;
        carry = product / base; // carry computation
        productRemainder = product % base; // remainder computation - will be saved in the result
        resultString.push_front(getCharacterFromDigit(productRemainder));
    }
    if (carry)
        resultString.push_front(getCharacterFromDigit(carry));
    return Number(resultString, base);
}

DivisionResult Number::operator/(const Number &otherNumber) {
    /* Overloading the / operator, this will help us to divide two numbers easier
     * INPUT: otherNumber: Number - the second term of division
     * OUTPUT: resultNumber: Number - the result of the integer division
     */
    QString currentNumberString = number, quotientString = "", remainderString;
    int secondDigitValue = getDigitFromCharacter(otherNumber.number[0]);
    int firstDivisionDigit = 0, secondDivisionDigit;
    int currentNumberStringSize = currentNumberString.size();
    int divisionRemainder, divisionQuotient, divisionNumber;
    for (int i = 0; i < currentNumberStringSize; ++i) {
        // we will compute the division using groups of 2 digits
        secondDivisionDigit = getDigitFromCharacter(currentNumberString[i]);
        divisionNumber = firstDivisionDigit * base + secondDivisionDigit; // number formed with those 2 digits
        divisionQuotient = divisionNumber / secondDigitValue;
        divisionRemainder = divisionNumber % secondDigitValue;
        quotientString.append(getCharacterFromDigit(divisionQuotient));
        firstDivisionDigit = divisionRemainder;
    }
    remainderString = QString(getCharacterFromDigit(divisionRemainder));
    return DivisionResult(quotientString, remainderString);
}

Number Number::operator=(const Number &otherNumber) {
    /* Assignment operator overloading
     */
    base = otherNumber.base;
    number = otherNumber.number;
    return *this;
}

bool Number::operator<(const Number &otherNumber) {
    /* Less operator overload, used for easier comparing
     */
    Number currentNumber = *this;
    QString currentNumberString = currentNumber.number, otherNumberString = otherNumber.number;
    if (currentNumberString == otherNumberString)
        return false;
    int currentNumberStringSize = currentNumberString.size(), otherNumberStringSize = otherNumberString.size();
    if (currentNumberStringSize > otherNumberStringSize)
        return false;
    makeNumberStringsUniform(currentNumberString, otherNumberString, currentNumberStringSize, otherNumberStringSize);
    for (int i = 0; i < currentNumberStringSize; ++i) {
        if (currentNumberString[i] < otherNumberString[i])
            return true;
    }
    return false;
}
