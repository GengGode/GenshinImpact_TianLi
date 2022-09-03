#ifdef _DEBUG

#include "TianLiQtCommon_Logger.h"

#include <QMutex>
#include <QDateTime>
#include <QTableWidget>


TianLiQtCommon_Logger& TianLiQtCommon_Logger::GetInstance()
{
	static QMutex mutex;
	static QScopedPointer<TianLiQtCommon_Logger> instance;
	if (instance.isNull())
	{
		mutex.lock();
		if (instance.isNull())
		{
			instance.reset(new TianLiQtCommon_Logger(NULL));
		}
		mutex.unlock();
	}
	return *instance.data();
}

TianLiQtCommon_Logger::TianLiQtCommon_Logger(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setAttribute(Qt::WA_QuitOnClose, false);
	this->setWindowFlags(Qt::Tool);

	ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);     
	ui.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);    
	ui.tableWidget->verticalHeader()->setVisible(false);                     
	ui.tableWidget->resizeColumnsToContents();                               
	ui.tableWidget->resizeRowsToContents();                                  
	ui.tableWidget->resizeColumnToContents(0);                               
	ui.tableWidget->setColumnCount(4);                                       
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	QHeaderView* widthlist = ui.tableWidget->horizontalHeader();             
	widthlist->setStyleSheet("QHeaderView::section{border:0px;background-color:#f2f1f7;color:#7f7f7f;}");
	widthlist->resizeSection(0, 180);		
	widthlist->resizeSection(1, 80);
	widthlist->resizeSection(2, 300);
	widthlist->resizeSection(3, 300);
	widthlist->setStretchLastSection(true);
	QStringList list;                                                     
	list << "Timekey" << "LogType" << "FuncAddr" << "Message" ;
	ui.tableWidget->setHorizontalHeaderLabels(list);
	ui.tableWidget->show();
	this->show();
}

TianLiQtCommon_Logger::~TianLiQtCommon_Logger()
{
}

void TianLiQtCommon_Logger::append(QString time, QString type, QString func, QString msg)
{
	int rowcount = ui.tableWidget->rowCount();           //获取当前行数
	ui.tableWidget->insertRow(rowcount);                 //新增行
	ui.tableWidget->setItem(rowcount, 0, new QTableWidgetItem(time)); //新增行添加数据
	QTableWidgetItem* item_type = new QTableWidgetItem(type);
	// Info Error Warning Debug other
	// blue red orange yellow green
	if (type == "Info")
	{
		item_type->setBackgroundColor(QColor(0, 0, 255, 100));
	}
	else if (type == "Error")
	{
		item_type->setBackgroundColor(QColor(255, 0, 0, 100));
	}
	else if (type == "Warning")
	{
		item_type->setBackgroundColor(QColor(255, 255, 0, 100));
	}
	else if (type == "Debug")
	{
		item_type->setBackgroundColor(QColor(0, 255, 0, 100));
	}
	else if(type=="Track")
	{
		item_type->setBackgroundColor(QColor(255, 255, 0,100));
	}
	else
	{
		item_type->setBackgroundColor(QColor(0, 255, 255, 100));
	}
	
	ui.tableWidget->setItem(rowcount, 1, item_type);
	ui.tableWidget->setItem(rowcount, 2, new QTableWidgetItem(func));
	ui.tableWidget->setItem(rowcount, 3, new QTableWidgetItem(msg));
}

void TianLiQtCommon_Logger::log(QString type, QString func, QString msg)
{
	QDateTime time = QDateTime::currentDateTime();  
	QString timestr = time.toString("yyyy-MM-dd hh:mm:ss:ms");             //设置显示格式
	append(timestr, type, func, msg);
}

void TianLiQtCommon_Logger::Trace(QString func, QString msg)
{
	log("Track", func, msg);
}

void TianLiQtCommon_Logger::Debug(QString func, QString msg)
{
	log("Debug", func, msg);
}

void TianLiQtCommon_Logger::Info(QString func, QString msg)
{
	log("Info", func, msg);
}

void TianLiQtCommon_Logger::Warn(QString func, QString msg)
{
	log("Warning", func, msg);
}

void TianLiQtCommon_Logger::Error(QString func, QString msg)
{
	log("Error", func, msg);
}

#endif