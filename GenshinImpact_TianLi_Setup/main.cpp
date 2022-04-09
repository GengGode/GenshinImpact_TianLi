#include "GenshinImpact_TianLi_Setup.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GenshinImpact_TianLi_Setup w;
    w.show();
    return a.exec();
}
