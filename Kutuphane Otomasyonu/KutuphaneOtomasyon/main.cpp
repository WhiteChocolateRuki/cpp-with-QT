#include "mainwindow.h"
#include <QApplication>
// şeklinde eklenen değerler yorum açıklması ve modüllerin ne işe yaradığını açıklansın diye eklenmiştir.
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
