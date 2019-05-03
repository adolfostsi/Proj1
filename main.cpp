#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/img/imgs/icon.jpg"));
    MainWindow w;
    w.setWindowTitle("F*** Transfer");
    w.setFixedSize(782,512);
    w.show();
    return a.exec();
}
