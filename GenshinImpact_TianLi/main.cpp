#include "GenshinImpact_TianLi.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    // 获取系统分辨率
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	// 如果分辨率高度大于1080 则放大两倍窗口显示，启用高分屏策略，否则正常显示
	//QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

	
    QApplication a(argc, argv);
    GenshinImpact_TianLi w;
    w.show();
    return a.exec();
}
