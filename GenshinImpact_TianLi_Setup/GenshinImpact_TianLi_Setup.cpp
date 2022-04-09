#include "GenshinImpact_TianLi_Setup.h"

#include <QMouseEvent>
#include <QFocusEvent>

GenshinImpact_TianLi_Setup::GenshinImpact_TianLi_Setup(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    installEventFilter(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    mainShadow_A = new QGraphicsDropShadowEffect(this);
    mainShadow_A->setOffset(0, 0);
    mainShadow_A->setColor(QColor(255, 255, 255));
    mainShadow_A->setBlurRadius(16);
    ui.label->setGraphicsEffect(mainShadow_A);

    mainShadow_B = new QGraphicsDropShadowEffect(this);
    mainShadow_B->setOffset(0, 0);
    mainShadow_B->setColor(QColor(255, 0, 0));
    mainShadow_B->setBlurRadius(14);
    ui.label_2->setGraphicsEffect(mainShadow_B);

    ui.stackedWidget->setCurrentIndex(0);
}

void GenshinImpact_TianLi_Setup::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton &&
		ui.label->frameRect().contains(event->globalPos() - this->frameGeometry().topLeft())) {
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
    return QWidget::eventFilter(object, event);
}