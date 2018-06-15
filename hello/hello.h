#ifndef HELLO_H
#define HELLO_H

#include <QtWidgets/QMainWindow>
#include <QStandardItemModel>
#include "ui_hello.h"
#include "HalconCpp.h"
#include "HandlePicThread.h"
#include "HalconCpp.h"
#include "Func.h"
#include "TestDialog.h"
#include "CameraThread.h"
#include "PylonCameraThread.h"

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
	void SetRightTableView();

	//*********图像处理线程**************/
	void HandleImageThread(HImage& ima, LocationView view);

public slots:

	/***********菜单栏功能************/
	void OnLRC();
	void OnAbout();

	/***********工具栏功能************/
	void OnOneHandle();
	void OnOpen();
	void OnLineRun();
	void OnConfigure();
	void OnShutDown();	//关闭系统
	void OnTest();

	/***********右端电气功能************/
	void DebugDialog();
	void ChangeMode(int mode);
	void OnStart();
	void OnStop();

	//
	void handleResults(bool is_bad);
	void readyDataSlot(QByteArray str);
	void TableSrolltoBottom();
	void receiveLeftImage(void* image);
	void receiveMiddleImage(void* image);
	void receiveRightImage(void* image);
	void receiveError(QString error);

private:
	void FullScreenShow();	//全屏显示
	void DispPic(HImage& path, LocationView location);	//显示图片在指定位置视图
	const HalconCpp::HTuple GetViewWindowHandle(LocationView location);	//返回指定视图的窗口Handle（Halcon显示图片用）
	void SetOpenWindowHandle(HImage& Image, HTuple* pWindowHandle, LocationView location);	//指定显示窗口的句柄
	void SetPicViewScroll(int width, int height, LocationView location); //指定视图的滚动条长度

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

public:
	Camera_Thread* m_camera_thread1;
	Camera_Thread* m_camera_thread2;
	
	PylonCamera_Thread* m_Pylon_camera_thread1;
	PylonCamera_Thread* m_Pylon_camera_thread2;
};

#endif // HELLO_H
