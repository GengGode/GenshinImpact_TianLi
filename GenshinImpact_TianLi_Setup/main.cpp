#include "GenshinImpact_TianLi_Setup.h"
#include <QtWidgets/QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    int font_id = QFontDatabase::addApplicationFont(":/font/resource/font/MiSans-Bold.ttf");
    QString font_name = QFontDatabase::applicationFontFamilies(font_id).at(0);
    QFont tianli_font(font_name, 15);
    a.setFont(tianli_font);
    
    GenshinImpact_TianLi_Setup w;
    w.show();
    return a.exec();
}
