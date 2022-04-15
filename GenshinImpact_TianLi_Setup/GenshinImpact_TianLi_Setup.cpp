#include "GenshinImpact_TianLi_Setup.h"

#include <QMouseEvent>
#include <QFocusEvent>

#include <QDebug>

#include "TianLiQtCommon_TimeLineLabel.h"

GenshinImpact_TianLi_Setup::GenshinImpact_TianLi_Setup(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    installEventFilter(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    //auto shadowLamda = [](const QColor& color = QColor(0, 0, 0), int radius = 16) {
    //    auto shadow = new QGraphicsDropShadowEffect;
    //    shadow->setOffset(0, 0);
    //    shadow->setColor(color);
    //    shadow->setBlurRadius(radius);
    //    return shadow;
    //};

    mainShadow = new QGraphicsDropShadowEffect();
    mainShadow->setOffset(0, 0);
    mainShadow->setColor(QColor(255, 255, 255));
    mainShadow->setBlurRadius(16);
    ui.label_MainShadow->setGraphicsEffect(mainShadow);

    mainShadowAnimation = new QPropertyAnimation(mainShadow, "color");
    mainShadowAnimation->setDuration(500);
    mainShadowAnimation->setEasingCurve(QEasingCurve::OutExpo);

    mainShadow_A = new QGraphicsDropShadowEffect();
    mainShadow_A->setOffset(0, 0);
    mainShadow_A->setColor(QColor(255, 255, 255));
    mainShadow_A->setBlurRadius(16);
    ui.label_MainShadow_A->setGraphicsEffect(mainShadow_A);

    mainShadow_B = new QGraphicsDropShadowEffect();
    mainShadow_B->setOffset(0, 0);
    mainShadow_B->setColor(QColor(255, 0, 0));
    mainShadow_B->setBlurRadius(14);
    ui.label_MainShadow_B->setGraphicsEffect(mainShadow_B);
   
    
    int y = 51;

    TianLiQtCommon_TimeLineLabel* timeLineLabel_1 = new TianLiQtCommon_TimeLineLabel();
    timeLineLabel_1->setParent(ui.stackedWidget->widget(1));
    timeLineLabel_1->setGeometry(102, y, 15, 124);
    timeLineLabel_1->setAction(true);
    timeLineLabel_1->setBegin(true);

    TianLiQtCommon_TimeLineLabel* timeLineLabel_2 = new TianLiQtCommon_TimeLineLabel();
    timeLineLabel_2->setParent(ui.stackedWidget->widget(1));
    timeLineLabel_2->setGeometry(102, y + 31 *1, 15, 124);
    timeLineLabel_2->setAction(true);


    TianLiQtCommon_TimeLineLabel* timeLineLabel_3 = new TianLiQtCommon_TimeLineLabel();
    timeLineLabel_3->setParent(ui.stackedWidget->widget(1));
    timeLineLabel_3->setGeometry(102, y + 31 *2, 15, 124);
    timeLineLabel_3->setAction(true);
    timeLineLabel_3->setChecked(true);

    TianLiQtCommon_TimeLineLabel* timeLineLabel_4 = new TianLiQtCommon_TimeLineLabel();
    timeLineLabel_4->setParent(ui.stackedWidget->widget(1));
    timeLineLabel_4->setGeometry(102, y+31*3, 15, 124);
    timeLineLabel_4->setEnd(true);


    connect(ui.pushButton_UI_Close, &QPushButton::clicked, this, &GenshinImpact_TianLi_Setup::pushButton_UI_Close);
    connect(ui.pushButton_UI_Mini, &QPushButton::clicked, this, &GenshinImpact_TianLi_Setup::pushButton_UI_Mini);
    connect(ui.pushButton_FastInstall, &QPushButton::clicked, this, &GenshinImpact_TianLi_Setup::pushButton_FastInstall);
    connect(ui.pushButton_CustomizeInstall, &QPushButton::clicked, this, &GenshinImpact_TianLi_Setup::pushButton_CustomizeInstall);
    connect(ui.pushButton_Finishing_Cancel, &QPushButton::clicked, this, &GenshinImpact_TianLi_Setup::pushButton_Finishing_Cancel);
    connect(ui.pushButton_Finished_Run, &QPushButton::clicked, this, &GenshinImpact_TianLi_Setup::pushButton_Finished_Run);
    connect(ui.pushButton_Finished_Exit, &QPushButton::clicked, this, &GenshinImpact_TianLi_Setup::pushButton_Finished_Exit);

    ui.stackedWidget->setCurrentIndex(1);
}

void GenshinImpact_TianLi_Setup::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton &&
		ui.label_MainShadow->frameRect().contains(event->globalPos() - this->frameGeometry().topLeft())) {
		m_Press = event->globalPos();
		leftBtnClk = true;
	}
	event->ignore();
}

void GenshinImpact_TianLi_Setup::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		leftBtnClk = false;
	}
	event->ignore();
}
void GenshinImpact_TianLi_Setup::mouseMoveEvent(QMouseEvent* event)
{
	if (leftBtnClk) {
		m_Move = event->globalPos();
		this->move(this->pos() + m_Move - m_Press);
		m_Press = m_Move;
	}
	event->ignore();
}
bool GenshinImpact_TianLi_Setup::eventFilter(QObject* object, QEvent* event)
{
    if (event->type() == QEvent::ActivationChange)
    {
        if (QApplication::activeWindow() != this)
        {
            // 焦点转移
            mainShadow_B->setEnabled(false);
        }
        else
        {
            //焦点转进
            mainShadow_B->setEnabled(true);
        }
    }
    if (event->type() == QEvent::Enter)
    {
        mainShadowAnimation->stop();
        mainShadowAnimation->setEndValue("#8b371e");
        mainShadowAnimation->start();
    }
    if (event->type() == QEvent::Leave)
    {
        mainShadowAnimation->stop();
        mainShadowAnimation->setEndValue("#DDDDDD");
        mainShadowAnimation->start();
    }
    return QWidget::eventFilter(object, event);
}
void GenshinImpact_TianLi_Setup::pushButton_UI_Close()
{
    //ui.label_MainShadow->hide();
    ui.label_MainShadow_A->hide();
    ui.label_MainShadow_B->hide();

    exitAnimation = new QPropertyAnimation(ui.label_MainShadow, "geometry");
    exitAnimation->setDuration(1000);
    exitAnimation->setEndValue(QRect(10 , 10, 500, 200));
    connect(exitAnimation, &QPropertyAnimation::valueChanged, [=]() {
        this->update();
        });
    connect(exitAnimation, &QPropertyAnimation::finished, [=]() {
        //this->close();
        });
    exitAnimation->start();
}

void GenshinImpact_TianLi_Setup::pushButton_UI_Mini()
{
    this->showMinimized();
}

void GenshinImpact_TianLi_Setup::pushButton_FastInstall()
{
    ui.stackedWidget->setCurrentIndex(1);

}

void GenshinImpact_TianLi_Setup::pushButton_CustomizeInstall()
{
    ui.stackedWidget->setCurrentIndex(2);
}

void GenshinImpact_TianLi_Setup::pushButton_Finishing_Cancel()
{
    ui.stackedWidget->setCurrentIndex(0);
}

void GenshinImpact_TianLi_Setup::pushButton_Finished_Run()
{
    this->close();
}

void GenshinImpact_TianLi_Setup::pushButton_Finished_Exit()
{
    this->close();
}
