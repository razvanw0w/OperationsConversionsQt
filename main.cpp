#include "userinterface.h"
#include <QApplication>
#include "number.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UserInterface window;
    window.show();

    return a.exec();
}
