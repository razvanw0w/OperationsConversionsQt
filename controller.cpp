#include "controller.h"
#include "divisionresult.h"
#include "number.h"
#include <QMessageBox>

Controller::Controller() {
    numberValidator = NumberValidator();
}

QString Controller::convertNumber(QString initialNumberString, int sourceBase, int destinationBase, bool intermediaryBase10) {
    /* This function converts a number given by its string representation
     * and its source base by the most adequate method:
     * when the source base is smaller than the destination base, we use
     *     the substitution method
     * when the source base is bigger than the destination base, we use
     *     the successive division method
     * INPUT: initialNumberString: QString
     *        sourceBase: int
     *        destinationBase: int
     *        intermediaryBase10: bool (True or False whether the user wants
     *                                  to pass through base 10 as an intermediary)
     * OUTPUT: convertedNumberString: QString
     */
    Number currentNumber = Number(initialNumberString, sourceBase);
    if (numberValidator.validateNumber(currentNumber) == false) {
        // the newly created number is not valid
        return "error";
    }
    QString convertedNumberString = "";
    if (sourceBase == destinationBase) {
        // no need to convert anything
        convertedNumberString = initialNumberString;
    }
    else if (intermediaryBase10 == true) {
        // must pass through base 10
        convertedNumberString = convertNumberByPassingThroughBase10(currentNumber, destinationBase);
    }
    else if (sourceBase < destinationBase) {
        // adequate case for the substitution method
        convertedNumberString = convertNumberBySubstitution(currentNumber, destinationBase);
    }
    else if (sourceBase > destinationBase) {
        // adequate case for the successive division method
        convertedNumberString = convertNumberBySuccessiveDivisions(currentNumber, destinationBase);
    }
    return convertedNumberString;
}

QString Controller::rapidlyConvertNumber(QString initialNumberString, int sourceBase, int destinationBase) {
    /* This function rapidly converts a number from a source base to a destination base, both being powers of 2
     * INPUT: initialNumberString: QString - the initial number
     *        sourceBase: int - the source base
     *        destinationBase: int - the destination base
     * OUTPUT: conversionResult: QString
     */
    Number currentNumber = Number(initialNumberString, sourceBase);
    if (numberValidator.validateNumber(currentNumber) == false) {
        return "error";
    }
    QString conversionResult = initialNumberString; // if the source base is 2, then the first step of
                                                    // the answer is trivial
    if (sourceBase != 2) {
        // firstly, we convert the number into base 2
        conversionResult = rapidlyConvertNumberIntoBase2(currentNumber);
        removeLeadingZeros(conversionResult); // we remove any leading zeroes
    }
    currentNumber = Number(conversionResult, 2);
    if (destinationBase != 2) {
        // being in base 2, we now convert the number into the destination base
        conversionResult = rapidlyConvertBase2NumberIntoDifferentBase(currentNumber, destinationBase);
        removeLeadingZeros(conversionResult);
    }
    return conversionResult;
}

QString Controller::convertNumberBySubstitution(Number number, int destinationBase) {
    /* This function converts a number by substitution to a destination base
     * INPUT: number: Number - the number which needs to be converted
     *        destinationBase: int - the destination base
     * OUTPUT: conversionResultString: QString
     */
    QString numberString = number.getNumber();
    int numberStringSize = numberString.size();

    Number power(QString(getCharacterFromDigit(1)), destinationBase); // initializing the power which will be constructed on the go
    Number conversionResult("0", destinationBase); // initially, the result is 0
    QChar currentCharacter;
    QString characterString;
    Number characterNumber;
    Number baseNumber = Number(QString(getCharacterFromDigit(number.getBase())), destinationBase); // the base which will we multiply the power by
    for (int i = numberStringSize - 1; i >= 0; --i) {
        currentCharacter = numberString[i];
        characterString = QString(currentCharacter);
        characterNumber = Number(characterString, destinationBase);
        conversionResult = conversionResult + (power * characterNumber); // adding to the result the product between the digit and its associated power
        power = power * baseNumber; // raising the power to the immediate superior power
    }
    return conversionResult.getNumber();
}

QString Controller::convertNumberBySuccessiveDivisions(Number number, int destinationBase) {
    /* This function converts a number by successive divisions to a destination base
     * INPUT: number: Number - the number which needs to be converted
     *        destinationBase: int - the destination base
     * OUTPUT: conversionResultString: QString
     */
    DivisionResult divisionResult;
    QString resultString = "";
    QString quotient, remainder;
    Number dividendNumber = number; // the dividend
    Number divisorNumber = Number(QString(getCharacterFromDigit(destinationBase)), number.getBase()); // the divisor
    while (dividendNumber.getNumber() != "0") { // while the dividend hasn't reached 0
        divisionResult = dividendNumber / divisorNumber; // execute the operation
        quotient = divisionResult.getQuotient();
        remainder = divisionResult.getRemainder();
        resultString.push_front(remainder); // we push the remainder at the beginning of the result
        removeLeadingZeros(quotient); // we remove the leading zeroes not to be confused by them
        dividendNumber = Number(quotient, number.getBase()); // the new dividend will be the quotient
    }
    return resultString;
}

QString Controller::convertNumberByPassingThroughBase10(Number number, int destinationBase) {
    /* This function converts a number by using base 10 as an intermediary to a destination base
     * INPUT: number: Number - the number which needs to be converted
     *        destinationBase: int - the destination base
     * OUTPUT: conversionResultString: QString
     */
    int numberBase = number.getBase();
    QString firstStepString = number.getNumber(), secondStepString;
    // firstly converting into base 10
    if (numberBase > 10) {
        firstStepString = convertNumberBySuccessiveDivisions(number, 10);
    }
    else if (numberBase < 10) {
        firstStepString = convertNumberBySubstitution(number, 10);
    }

    // being in base 10, now we convert the number into the desired base
    number = Number(firstStepString, 10);
    secondStepString = firstStepString;
    if (destinationBase > 10) {
        secondStepString = convertNumberBySubstitution(number, destinationBase);
    }
    else if (destinationBase < 10) {
        secondStepString = convertNumberBySuccessiveDivisions(number, destinationBase);
    }
    return secondStepString;
}

QString Controller::rapidlyConvertNumberIntoBase2(Number number) {
    /* This function rapidly converts a number in base 4/8/16 into base 2
     * INPUT: number: Number - the number which needs to be converted
     * OUTPUT: resultString: QString
     */
    QString numberString = number.getNumber();
    QString resultString = "", temporaryString = "", characterString;
    Number singleDigitNumber;
    int numberStringSize = numberString.size();
    int base = number.getBase();

    for (int i = numberStringSize - 1; i >= 0; --i) {
        characterString = QString(numberString[i]);
        singleDigitNumber = Number(characterString, base);
        // we replace every non-base2 digit with its base 2 representation
        // it will be replaced by a 2-digit, 3-digit or 4-digit binary number
        // depending if the initial number is in base 4, 8 or 16, respectively
        temporaryString = convertPowerOf2BaseDigitIntoBinary(numberString[i], base);
        // because we are checking the string from right to left, we are appending
        // the result at the front of the result string
        resultString.push_front(temporaryString);
    }
    return resultString;
}

QString Controller::rapidlyConvertBase2NumberIntoDifferentBase(Number number, int destinationBase) {
    /* This function converts a binary number into base 4/8/16
     * INPUT: number: Number - the number which needs to be converted
     *        destinationBase: int - the destination base
     * OUTPUT: resultString: QString
     */
    QString numberString = number.getNumber();
    int numberStringSize = numberString.size();
    QString resultString = "", characterString;
    QChar character;

    // gap - the number of elements of each group of binary digits
    int gap, temporaryResult;
    switch (destinationBase) {
        case 4: gap = 2; break; // a base 4 number consists in 2 binary digits
        case 8: gap = 3; break; // a base 8 number consists in 3 binary digits
        case 16: gap = 4; break; // a base 16 number consists in 4 binary digits
    }
    for (int i = numberStringSize - 1; i >= 0; i -= gap) {
        // we pass through the string from gap to gap
        temporaryResult = 0;
        for (int j = 0; i - j >= 0 && j < gap; ++j) {
            // we traverse the current group, each group has 'gap' elements
            if (numberString[i - j] == '1') {
                if (j == 0)
                    temporaryResult += 1;
                else if (j == 1)
                    temporaryResult += 2;
                else if (j == 2)
                    temporaryResult += 4;
                else if (j == 3)
                    temporaryResult += 8;
            }
        }
        character = getCharacterFromDigit(temporaryResult); // the temporary result is being converted to a character
        characterString = QString(character);
        resultString.push_front(characterString); // we push the resulting string to the front, as we are
                                                  // traversing the binary string from right to left
    }
    return resultString;
}

QString Controller::convertPowerOf2BaseDigitIntoBinary(QChar digit, int base) {
    int gap; // gap - the number of elements of each group of binary digits
    if (base == 16)
        gap = 4;
    else if (base == 8)
        gap = 3;
    else if (base == 4)
        gap = 2;
    QString numberString = QString(digit);
    QString resultString = convertNumberBySuccessiveDivisions(Number(numberString, base), 2); // we convert the number into base 2
    if (resultString.size() != gap) // we fill the digits with zeros to the left
        resultString.push_front(QString(gap - resultString.size(), '0'));
    return resultString;
}

QString Controller::executeArithmeticOperation(QString firstNumberString, QString secondNumberString, int base, QChar operation) {
    /* This function executes the desired arithmetic operation by the user between two numbers in the same base
     * INPUT: firstNumberString: QString - the first number in a string form
     *        secondNumberString: QString - the second number in a string form
     *        base: int - the base of the two numbers
     *        operation: QChar - the operation which needs to be executed
     * OUTPUT: resultString - the result of the operation
     */
    Number firstNumber(firstNumberString, base);
    Number secondNumber(secondNumberString, base);
    if (numberValidator.validateNumber(firstNumber) == false or numberValidator.validateNumber(secondNumber) == false)
        return "error";
    Number resultNumber;
    DivisionResult divisionResult;
    char operationChar = operation.toLatin1();
    if (operationChar == '+') {
        resultNumber = firstNumber + secondNumber;
        return resultNumber.getNumber();
    }
    if (operationChar == '-') {
        if (firstNumber < secondNumber)
            return "subtractionError";
        resultNumber = firstNumber - secondNumber;
        return resultNumber.getNumber();
    }
    if (operationChar == '*') {
        if (numberValidator.testIfValidForMultiplicationOrDivision(secondNumberString) == false)
            return "multiplicationDivisionError";
        resultNumber = firstNumber * secondNumber;
        return resultNumber.getNumber();
    }
    if (operationChar == '/') {
        if (numberValidator.testIfValidForMultiplicationOrDivision(secondNumberString) == false)
            return "multiplicationDivisionError";
        if (secondNumberString == "0")
            return "divisionByZero";
        divisionResult = firstNumber / secondNumber;
        return divisionResult.getQuotient() + " r " + divisionResult.getRemainder();
    }
}

int Controller::getDigitFromCharacter(QChar character)
{
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

QChar Controller::getCharacterFromDigit(int digit) {
    /* This function converts a number to its associated character
     * INPUT: digit - int
     * OUTPUT: character - QChar
     */
    QChar character = digitLookupTable[digit];
    return character;
}

void Controller::removeLeadingZeros(QString &numberString) {
    /* This function is used to remove the leading zeroes
     * INPUT: numberString - QString&
     * OUTPUT: nothing, but the string will be modified accordingly
     */
    int leadingZeroesCounter = 0;
    for (int i = 0; i < numberString.size() - 1; ++i) {
        if (numberString[i] == '0')
            ++leadingZeroesCounter;
        else
            break;
    }
    numberString.remove(0, leadingZeroesCounter);
}
