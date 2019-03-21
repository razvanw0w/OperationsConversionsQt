#include "userinterface.h"
#include "ui_userinterface.h"
#include <QMessageBox>

UserInterface::UserInterface(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserInterface)
{
    ui->setupUi(this);
    this->controller = Controller();
}

UserInterface::~UserInterface()
{
    delete ui;
}

void UserInterface::on_basicConversionPushButton_clicked()
{
    QString initialNumberString = ui->basicConversionInitialNumberTextbox->text();
    if (initialNumberString == "") {
        QMessageBox::critical(this, "Error!", "You didn't enter any number in the specified field.");
        return;
    }
    int sourceBase = ui->basicConversionSourceBaseCombobox->currentText().toInt();
    int destinationBase = ui->basicConversionDestinationBaseCombobox->currentText().toInt();
    bool passingThroughBase10 = ui->intermediaryBaseCheckbox->checkState() == Qt::CheckState::Checked;

    QString convertedNumberString = controller.convertNumber(initialNumberString, sourceBase, destinationBase, passingThroughBase10);
    if (convertedNumberString == "error") {
        QMessageBox::critical(this, "Error!", "The given number cannot be written in the specified base!");
    }
    else {
         ui->basicConversionResultNumberTextbox->setText(convertedNumberString);
    }
}

void UserInterface::on_basicOperationPushButton_clicked()
{
    QString firstNumberString = ui->basicOperationFirstNumberTextbox->text();
    if (firstNumberString == "") {
        QMessageBox::critical(this, "Error!", "You didn't enter any number as the first number.");
        return;
    }
    QString secondNumberString = ui->basicOperationSecondNumberTextbox->text();
    if (secondNumberString == "") {
        QMessageBox::critical(this, "Error!", "You didn't enter any number as the second number.");
        return;
    }
    int base = ui->basicOperationBaseCombobox->currentText().toInt();
    QChar operation = ui->basicOperationOperationCombobox->currentText()[0];
    QString operationResultString = controller.executeArithmeticOperation(firstNumberString, secondNumberString, base, operation);
    if (operationResultString == "error") {
        QMessageBox::critical(this, "Error!", "The given numbers cannot be written in the specified base!");
    }
    else if (operationResultString == "subtractionError") {
        QMessageBox::critical(this, "Error!", "The first number is less than the second one!");
    }
    else if (operationResultString == "multiplicationDivisionError") {
        QMessageBox::critical(this, "Error!", "The second number has more than one digit!");
    }
    else if (operationResultString == "divisionByZero") {
        QMessageBox::critical(this, "Error!", "Division by zero is forbidden!");
    }
    else {
        removeLeadingZeros(operationResultString);
        ui->basicOperationResultTextbox->setText(operationResultString);
    }
}

void UserInterface::removeLeadingZeros(QString &numberString) {
    /* This function is used to remove the leading zeroes
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

void UserInterface::on_rapidConversionPushButton_clicked()
{
    QString initialNumberString = ui->rapidConversionInitialNumberTextbox->text();
    if (initialNumberString == "") {
        QMessageBox::critical(this, "Error!", "You didn't enter any number in the specified field.");
        return;
    }
    int sourceBase = ui->rapidConversionSourceBaseCombobox->currentText().toInt();
    int destinationBase = ui->rapidConversionDestinationBaseCombobox->currentText().toInt();

    QString rapidlyConvertedNumberString = controller.rapidlyConvertNumber(initialNumberString, sourceBase, destinationBase);
    if (rapidlyConvertedNumberString == "error") {
        QMessageBox::critical(this, "Error!", "The given number cannot be written in the specified base!");
    }
    else {
         ui->rapidConversionResultNumberTextbox->setText(rapidlyConvertedNumberString);
    }
}
