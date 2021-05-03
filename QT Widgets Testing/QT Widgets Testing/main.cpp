#include "QTWidgetsTesting.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTWidgetsTesting w;
    w.show();
    return a.exec();
}
