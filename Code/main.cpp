#include "mainwindow.h"
#include "client.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    Client obj;
    QApplication a(argc, argv);
    MainWindow w(&obj);
    w.show();
    return a.exec();
}
