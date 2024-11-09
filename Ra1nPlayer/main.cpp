#include "mainwidget.h"
#include <QtWidgets/QApplication>

#undef main
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget w;
    w.show();
    return a.exec();
}
