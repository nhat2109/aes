#include "mainwindow.h"

#include <QApplication>

#include <QMainWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Phần mềm chữ ký điện tử");
    w.show();
    return a.exec();
}
