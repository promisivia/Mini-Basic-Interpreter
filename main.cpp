#include "basic.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Basic w;
    w.show();

    return a.exec();
}
