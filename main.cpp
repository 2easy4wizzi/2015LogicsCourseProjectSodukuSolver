#include "widget.h"
#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget mainPage;
    mainPage.show();
    return a.exec();
}
