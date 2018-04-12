#ifndef HELLO_H
#define HELLO_H

#include <QtWidgets/QMainWindow>
#include <QStandardItemModel>
#include "ui_hello.h"
#include "HalconCpp.h"
#include "HandlePicThread.h"
#include "HalconCpp.h"
#include "Func.h"
#include "CommDialog.h"

using namespace HalconCpp;

class hello : public QMainWindow
{
	Q_OBJECT

public:
	hello(QWidget *parent = 0);
	~hello();

	bool getIsBadStatu(){ return m_bIsBad; };
	void AddItemToTableView(QTableView* view, QStandardItemModel* model, 
		QStandardItem* item0, QStandardItem* item1, QStandardItem* item2, QStandardItem* item3,
		const QBrush& textcolor, int RowHeight = 20);


public slots:
	void click();
	void Open();
	void About();
	void OnLineRun();
	void OnConfigure2();
	void OnShutDown();	//关闭系统
	void handleResults(bool is_bad);
	void readyDataSlot(QByteArray str);
	void TableSrolltoBottom(); 

	/***********右端调试功能************/
	void DebugDialog();
	void ChangeMode(int mode);
	void start();
	void stop();
	/***********右端调试功能************/



private:
	void DispPic(HImage& path, LocationView location);	//显示图片在指定位置视图
	const HalconCpp::HTuple GetViewWindowHandle(LocationView location);	//返回指定视图的窗口Handle（Halcon显示图片用）

	CommDialog* pDlg;
	QStandardItemModel* Right_dataModel;

private:
	Ui::helloClass ui;
	HalconCpp::HTuple m_LeftWindowHandle, m_MiddleWindowHandle, m_RightWindowHandle;	//左、中、右视图的窗口Handle（Halcon显示图片用）
	QString m_Title;
	HandlePicThread *m_pHandlePicThread;
	HImage m_Image;
	bool m_bIsBad;

signals:
	void ReadyLoop();
	
};

#endif // HELLO_H
