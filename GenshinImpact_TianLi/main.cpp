#include "GenshinImpact_TianLi.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    // ��ȡϵͳ�ֱ���
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	// ����ֱ��ʸ߶ȴ���1080 ��Ŵ�����������ʾ�����ø߷������ԣ�����������ʾ
	//QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

	
    QApplication a(argc, argv);
    GenshinImpact_TianLi w;
    w.show();
    return a.exec();
}
