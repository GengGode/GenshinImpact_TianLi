#include "GenshinImpact_TianLi.h"
#include <QtWidgets/QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    // 获取系统分辨率
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	// 如果分辨率高度大于1080 则放大两倍窗口显示，启用高分屏策略，否则正常显示
	//QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
		// 设置字体 :/Font/resource/Font/YuanShenFont.ttf

    QApplication a(argc, argv);

	int font_id = QFontDatabase::addApplicationFont(":/Font/resource/Font/YuanShenFont.ttf");
	QString font_name = QFontDatabase::applicationFontFamilies(font_id).at(0);
	QFont tianli_font(font_name, 15);
	a.setFont(tianli_font);

	QCursor tianli_cursor(QPixmap(":/Cursor/resource/Cursor/UI_Img_Cursor_PC.png"), 0, 0);
	a.setOverrideCursor(tianli_cursor);
	
	GenshinImpact_TianLi w;
    w.show();
    return a.exec();
}
