#include "numbervalidator.h"

NumberValidator::NumberValidator()
{}

bool NumberValidator::validateNumber(Number number) {
    /* This function validates a number object.
     * A number object must have a valid number as a string
     * and moreover, it should be possible to write that
     * certain number in the specific base.
     * INPUT: number: Number
     * OUTPUT: True if the number is valid
     *         False otherwise
     */
    QString numberString = number.getNumber();
    int base = number.getBase();
    return testIfNumberIsInBase(numberString, base) == true;
}

bool NumberValidator::testIfNumberIsInBase(QString numberString, int base)
{
    /* This function tests if a certain number is in a base
     * INPUT: numberString: QString
     *        base: int
     * OUTPUT: True if the string contains only digits which can be
     *              used to write that number in the given base
     *         False otherwise
     */
    QString allowedHexadecimalDigits = "ABCDEF";
    for (QChar& character: numberString) {
        if (character.isDigit()) {
            // if the character is a digit, its designated value
            // should be less than the base it must be written in
            if (character.toLatin1() - '0' >= base) {
                return false;
            }
        }
        else if (character.isLetter()) {
            if (base == 16) {
                // if the base is 16, then the only letters in the string
                // should be from "ABCDEF"
                if (character.isLower()) {
                    character.toUpper();
                }
                if (allowedHexadecimalDigits.contains(character) == false)
                    return false;
            }
            else // if the base isn't 16 then there shouldn't be any digits there
                return false;
        }
        else
            return false;
    }
    return true;
}

bool NumberValidator::testIfValidForMultiplicationOrDivision(QString numberString)
{
    /* We must multiply or divide with only single digit
     */
    if (numberString.size() != 1)
        return false;
    return true;
}
