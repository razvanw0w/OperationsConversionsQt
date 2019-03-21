#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QMainWindow>
#include "controller.h"

namespace Ui {
    class UserInterface;
}

class UserInterface : public QMainWindow {
    Q_OBJECT

public:
    explicit UserInterface(QWidget *parent = 0);
    ~UserInterface();

private slots:
    void on_basicConversionPushButton_clicked();

    void on_basicOperationPushButton_clicked();

    void on_rapidConversionPushButton_clicked();

private:
    Ui::UserInterface *ui;
    Controller controller;
    void removeLeadingZeros(QString& numberString);
};

#endif // USERINTERFACE_H
