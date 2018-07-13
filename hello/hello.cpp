#include "hello.h"
#include <qfiledialog.h>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QThread>
#include "CHH.h"
#include "aboutdialog.h"
#include "configuredlg.h"
#include "ComDialog.h"
#include "DeltaThread.h"
#include "ConstParam.h"
#include "ConfigureDialog.h"



hello::hello(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	m_LeftWindowHandle = m_MiddleWindowHandle = m_RightWindowHandle = 0;
	// 菜单栏对应功能
	connect(ui.About, &QAction::triggered, this, &hello::OnAbout);
	connect(ui.OnLRC, &QAction::triggered, this, &hello::OnLRC);
	//工具栏对应功能
	connect(ui.Open, &QAction::triggered, this, &hello::OnOpen);
	connect(ui.OnRun, &QAction::triggered, this, &hello::OnOneHandle);
	connect(ui.OnLineRun, SIGNAL(triggered()), this, SLOT(OnLineRun()));
	connect(ui.OnStop, SIGNAL(triggered()), this, SLOT(OnStop()));
	connect(ui.Configure, SIGNAL(triggered()), this, SLOT(OnConfigure()));
	connect(ui.ShutDown, SIGNAL(triggered()), this, SLOT(OnShutDown()));
	connect(ui.Configure2, SIGNAL(triggered()), this, SLOT(OnTest()));
	//右端界面对应功能
	connect(ui.btn_debug, SIGNAL(clicked()), this, SLOT(DebugDialog()));
	connect(ui.btn_start, SIGNAL(clicked()), this, SLOT(OnStart()));
	connect(ui.btn_stop, SIGNAL(clicked()), this, SLOT(OnStop()));
	connect(ui.verticalSlider_mode, SIGNAL(valueChanged(int)), this, SLOT(ChangeMode(int)));
	ui.btn_debug->setEnabled(false);
	ui.btn_stop->setEnabled(false);

	WriteCurrenDateTime("config.ini", "Config", "OpenProgramTime");  //不支持中文写入配置文件
	
	SetRightTableView();

	//定时刷新列表滚动条至底部
	QTimer* timer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(TableSrolltoBottom()));
	timer->start(100);
	
	//HIDDLE_DIALOG_BUTTON
	FullScreenShow();	//全屏显示

	m_camera_thread1 = nullptr;
	m_camera_thread2 = nullptr;
	m_Pylon_camera_thread1 = nullptr;
	m_Pylon_camera_thread2 = nullptr;
	
	//std::string str = ":00030C03E8000000000000003200005B\r\n";
	std::string str = ":00030C043F00000000000000320";
	std::vector<ushort> X_Status = Parse_Delta_Ascii_03(str);

}

//全屏显示
void hello::FullScreenShow()
{
	QRect rect = qApp->desktop()->geometry();
	setGeometry(qApp->desktop()->geometry());
	setContentsMargins(0, 0, 0, 0);
}

//设置右视图内容
void hello::SetRightTableView()
{
	Right_dataModel = new QStandardItemModel();
	ui.tableView_right->setModel(Right_dataModel);  //绑定数据模型
	ui.tableView_right->setShowGrid(false);

	//设置列表头
	QStringList labels = (G2U("时间,对象名,报警类型,报警事件,当前值,界限值,报警描述")).simplified().split(",");
	Right_dataModel->setHorizontalHeaderLabels(labels);

	ui.tableView_right->setColumnWidth(0, 150);
	ui.tableView_right->setColumnWidth(1, 80);
	ui.tableView_right->setColumnWidth(2, 80);
	ui.tableView_right->setColumnWidth(3, 80);
	ui.tableView_right->setColumnWidth(4, 80);
	ui.tableView_right->setColumnWidth(5, 80);
}

//列表滚动条至底部
void hello::TableSrolltoBottom()
{
		
	if (!ui.tableView_right->hasFocus())
		ui.tableView_right->scrollToBottom();

}

hello::~hello()
{

}

void hello::OnAbout()
{
	AboutDialog Dlg(this);
	Dlg.exec();
}

void hello::OnLRC()
{
	ComDialog Dlg(this);
	Dlg.exec();
}

//创建线程
void hello::OnOneHandle()
{
	HandlePicThread* m_pHandlePicThread = new HandlePicThread(this);
	m_pHandlePicThread->m_Image = m_Image;
	m_pHandlePicThread->m_WindowHandle = GetViewWindowHandle(RightView);

	connect(m_pHandlePicThread, SIGNAL(resultReady(bool)), this, SLOT(handleResults(bool)));
	// 线程结束后，自动销毁
	connect(m_pHandlePicThread, SIGNAL(finished()), m_pHandlePicThread, SLOT(deleteLater()));
	m_pHandlePicThread->start();
}

void hello::OnShutDown()
{
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, G2U("系统"), G2U("确认退出系统"));
	if (reply == QMessageBox::StandardButton::Yes)
	{
		if (m_camera_thread1)
			m_camera_thread1->stop();

		if (m_camera_thread2)
			m_camera_thread2->stop();

		if (m_Pylon_camera_thread1)
			m_Pylon_camera_thread1->stop();

		if (m_Pylon_camera_thread2)
			m_Pylon_camera_thread2->stop();

		if (Camera_Thread::IsExistCameraId(LineCameraId_Dalsa1))
		{
			if (m_camera_thread1->isRunning())
			{
				m_camera_thread1->stop();
				m_camera_thread1->wait();
			}
		}

		if (Camera_Thread::IsExistCameraId(LineCameraId_Dalsa2))
		{
			if (m_camera_thread2->isRunning())
			{
				m_camera_thread2->stop();
				m_camera_thread2->wait();
			}
		}

		if (PylonCamera_Thread::IsExistCameraId(LineCameraId_Pylon_Basler1))
		{
			if (m_Pylon_camera_thread1->isRunning())
			{
				m_Pylon_camera_thread1->stop();
				m_Pylon_camera_thread1->wait();
			}
		}

		if (PylonCamera_Thread::IsExistCameraId(LineCameraId_Pylon_Basler2))
		{
			if (m_Pylon_camera_thread2->isRunning())
			{
				m_Pylon_camera_thread2->stop();
				m_Pylon_camera_thread2->wait();
			}
		}
		//m_camera_thread1->destroyed();
		//m_camera_thread1->exit();
		//close();
		qApp->quit();
	}
	
}

void hello::OnConfigure()
{
	/*ConfigureDlg Dlg(this);
	Dlg.exec();*/

	ConfigureDialog Dlg(this);
	Dlg.exec();
}

//得到线程的信号
void hello::handleResults(bool is_bad)
{
	if (m_RightWindowHandle != 0)
		m_bIsBad = is_bad;	
}


void hello::OnLineRun()
{
	QVariant Value;
	ReadConfigure("config.ini", "Port", "Port", Value);
	QString port = Value.toString();
	ReadConfigure("config.ini", "Port", "Baud", Value);
	int baud = Value.toInt();
	statusBar()->showMessage(port + "," + Value.toString());



}

//对应串口的读取槽函数
void hello::readyDataSlot(QByteArray str)  
{	
	//qDebug() << "MainSlot Thread : " << QThread::currentThreadId();
	statusBar()->showMessage(str);	
	QDateTime curTime;
	if (Right_dataModel->rowCount() == 100)
	{
		Right_dataModel->setData(Right_dataModel->index(0, 0), curTime.currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
		Right_dataModel->setData(Right_dataModel->index(0, 1), QString(str));
		Right_dataModel->sort(0, Qt::AscendingOrder);
	
	}
	else
	{
		AddItemToTableView(ui.tableView_right, Right_dataModel,
			new QStandardItem(curTime.currentDateTime().toString("yyyy-MM-dd hh:mm:ss")),
			new QStandardItem(QString(str)),
			new QStandardItem(G2U("上限警报")),
			new QStandardItem(G2U("报警结束")),
			QColor(0, 0, 0));
	}
	
}

//添加信息至表，自动增行
void hello::AddItemToTableView(QTableView* view, QStandardItemModel* model, QStandardItem* item0, QStandardItem* item1, QStandardItem* item2, QStandardItem* item3, const QBrush& textcolor, int RowHeight)
{
	int Row = model->rowCount();

	model->setItem(Row, 0, item0);
	model->item(Row, 0)->setTextAlignment(Qt::AlignCenter);	//设置数据居中显示
	model->item(Row, 0)->setForeground(textcolor); //设置数据显示颜色

	model->setItem(Row, 1, item1);
	model->item(Row, 1)->setTextAlignment(Qt::AlignCenter);	
	model->item(Row, 1)->setForeground(textcolor); 

	model->setItem(Row, 2, item2);
	model->item(Row, 2)->setTextAlignment(Qt::AlignCenter);	
	model->item(Row, 2)->setForeground(textcolor); 

	model->setItem(Row, 3, item3);
	model->item(Row, 3)->setTextAlignment(Qt::AlignCenter);	
	model->item(Row, 3)->setForeground(textcolor); 

	view->setRowHeight(Row, RowHeight);

	model->sort(0, Qt::AscendingOrder);

}

void hello::OnOpen()
{

	QString path = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files(*.jpg *.png *.bmp)"));

	if (path.contains("camera1") || path.contains("camera2"))
	{
		int i =path.lastIndexOf('/');
		QString ImageName = path.toStdString().substr(i, path.length() - 1).c_str();
		QString tempPath = path.remove(ImageName);
		i = tempPath.lastIndexOf('/');
		QString DateName = tempPath.toStdString().substr(0, i+1).c_str();
		
		QString ImagePath = DateName + "camera1" + ImageName;
		ReadImage(&m_Image, ImagePath.toLocal8Bit().constData());
		DispPic(m_Image, LeftView);

		ImagePath = DateName + "camera2" + ImageName;
		ReadImage(&m_Image, ImagePath.toLocal8Bit().constData());
		DispPic(m_Image, RightView);
	}
	else
	{
		if (path.length() != 0 &&
			(path.contains(".bmp") || path.contains(".jpg")))
		{
			ReadImage(&m_Image, path.toLocal8Bit().constData());
			DispPic(m_Image, RightView);
		}
	}
	
}


void hello::DispPic(HImage& Image, LocationView location)
{
	int width = Image.Width();
	int height = Image.Height();
	HTuple* pWindowHandle = nullptr;
	switch (location)
	{
		case RightView:
			pWindowHandle = &m_RightWindowHandle;;
			break;

		case LeftView:
			pWindowHandle = &m_LeftWindowHandle;
			break;

		case MiddleView:
			pWindowHandle = &m_MiddleWindowHandle;
			break;

	}
	/*if (location == LeftView)
	{
		pWindowHandle = &m_LeftWindowHandle;
	}
	else if(location == MiddleView)
	{
		pWindowHandle = &m_MiddleWindowHandle;
	}
	else
	{
		pWindowHandle = &m_RightWindowHandle;
	}*/

	if (*pWindowHandle == 0)
	{
		
		SetOpenWindowHandle(Image, pWindowHandle, location);
		DispObj(Image, *pWindowHandle);
		SetPicViewScroll(width, height, location);

	}
	else
	{
		//ClearWindow(*pWindowHandle);
		//CloseWindow(*pWindowHandle);

		//SetOpenWindowHandle(Image, pWindowHandle, location);
		DispObj(Image, *pWindowHandle);
		SetPicViewScroll(width, height, location);
	}

}

//指定显示窗口的句柄
void hello::SetOpenWindowHandle(HImage& Image, HTuple* pWindowHandle, LocationView location)
{
	switch (location)
	{
	case RightView:
		CHH::dev_open_window_fit_image(Image, 0, 0, 1024, 4000, (long)ui.RightPicView->winId(), pWindowHandle);
		break;

	case LeftView:
		CHH::dev_open_window_fit_image(Image, 0, 0, 1024, 4000, (long)ui.LeftPicView->winId(), pWindowHandle);
		break;

	case MiddleView:
		CHH::dev_open_window_fit_image(Image, 0, 0, 1024, 4000, (long)ui.MiddlePicView->winId(), pWindowHandle);
		break;
	}
}

//指定视图的滚动条长度
void hello::SetPicViewScroll(int width, int height, LocationView location)
{
	float scaleX = width / 1024.0;
	float scaleY = height / 4000.0;
	switch (location)
	{
	case RightView:
		if (scaleY > scaleX)
		{
			HSCROLL_HEIGHT_RightPic(4000);
			ui.scrollAreaWidgetContentsRight->setMinimumWidth(850);
		}
		else
		{
			HSCROLL_HEIGHT_RightPic(500);
		}
		break;
	case LeftView:
		if (scaleY > scaleX)
		{
			HSCROLL_HEIGHT_LeftPic(4000);
			ui.scrollAreaWidgetContentsLeft->setMinimumWidth(850);
		}
		else
		{
			HSCROLL_HEIGHT_LeftPic(500);
		}
		break;
	case MiddleView:
		if (scaleY > scaleX)
		{
			HSCROLL_HEIGHT_MiddlePic(4000);
			ui.scrollAreaWidgetContentsMiddle->setMinimumWidth(850);
		}
		else
		{
			HSCROLL_HEIGHT_MiddlePic(500);
		}
		break;
	}
}


const HalconCpp::HTuple hello::GetViewWindowHandle(LocationView location)
{
	if (location == RightView)
	{
		return m_RightWindowHandle;
	}
	else if (location == MiddleView)
	{
		return m_MiddleWindowHandle;
	}
	else
	{
		return m_LeftWindowHandle;
	}
}

/***********右端调试功能************/
//打开调试界面
void hello::DebugDialog()
{
	TestDialog dlg(this);
	dlg.exec();
}

//启动
void hello::OnStart()
{/*
	ui.btn_start->setEnabled(false);
	QVariant value;
	ReadConfigure("config.ini", "Port", "Port", value);
	QString PortName = value.toString();
	ReadConfigure("config.ini", "Port", "Baud", value);
	int Baud = value.toInt();
	ReadConfigure("config.ini", "Port", "DataBits", value);
	int DataBits = value.toInt();

	//设置默认查询队列
	Delta_Thread::setQueryMode(Delta_Thread::QueryMode::DefalutQuene);
	/*Delta_Thread::AddDefaultQueueInfo("00050500FF00");
	Delta_Thread::AddDefaultQueueInfo("00050501FF00");
	Delta_Thread::AddDefaultQueueInfo("00050502FF00");
	Delta_Thread::AddDefaultQueueInfo("00050503FF00");
	Delta_Thread::AddDefaultQueueInfo("00050504FF00");*/
	Delta_Thread::AddDefaultQueueInfo("000105000028");	//读X00-X47
	Delta_Thread::AddDefaultQueueInfo("000204000028");	//读Y00-Y47
	/*
	if (!Delta_Thread::GetSerialPort())
	{
		Delta_Thread* thread = new Delta_Thread;
		thread->InitSerialPortInfo(PortName.toStdString().c_str(), Baud, QSerialPort::Parity::EvenParity, QSerialPort::DataBits(DataBits));
		connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
		connect(thread, SIGNAL(emitdata(QByteArray)), this, SLOT(readyDataSlot(QByteArray)));
		thread->start();
		ui.btn_stop->setEnabled(true);
		ui.btn_debug->setEnabled(false);
		ui.verticalSlider_mode->setEnabled(false);
	}
	*/
}

//停止
void hello::OnStop()
{
	Delta_Thread::StopRun(true);

	ui.btn_start->setEnabled(true);
	ui.btn_stop->setEnabled(false);
	ui.verticalSlider_mode->setEnabled(true);
}

//模式切换
void hello::ChangeMode(int mode)
{
	if (mode == 1)
	{
		ui.btn_debug->setEnabled(false);
		ui.btn_start->setEnabled(true);
	}
	else
	{
		ui.btn_debug->setEnabled(true);
		ui.btn_start->setEnabled(false);
	}
}



//******
void hello::receiveLeftImage(void* image)
{
	HImage ima = *(HImage*)image;
	DispPic(ima, LeftView);
	HandleImageThread(ima, LeftView);
}

void hello::receiveRightImage(void* image)
{
	HImage ima = *(HImage*)image;
	DispPic(ima, RightView);
	HandleImageThread(ima, RightView);
	
}

void hello::receiveMiddleImage(void* image)
{
	HImage ima = *(HImage*)image;
	DispPic(ima, MiddleView);
}

void hello::receiveError(QString error)
{
	QMessageBox::StandardButton reply;
	reply = QMessageBox::warning(this, G2U("错误"), error);
}

void hello::OnTest()
{
	QVariant value;
	ReadConfigure("config.ini", "Config", "ImagePath1", value);

	/*-----Halcon Version------------*/
	//m_camera_thread1 = new Camera_Thread(Camera_Thread::ConnectionType::DirectShow, AreaCamera880Id, this);
	//m_camera_thread1->setSaveDatePath(value.toString());
	//m_camera_thread1->setSaveImageDirName("Camera1");
	//m_camera_thread1->setAalConfigureName("Camera1");
	//m_camera_thread1->setSaveImageNum(50);
	//connect(m_camera_thread1, SIGNAL(signal_image(void*)), this, SLOT(receiveLeftImage(void*)));
	//connect(m_camera_thread1, SIGNAL(signal_error(QString)), this, SLOT(receiveError(QString)));
	//connect(m_camera_thread1, SIGNAL(finished()), m_camera_thread1, SLOT(deleteLater()));
	//m_camera_thread1->start();

	/*m_camera_thread1 = new Camera_Thread(Camera_Thread::ConnectionType::GigEVision, LineCameraId_Basler1, this);
	m_camera_thread1->setSaveDatePath(value.toString());
	m_camera_thread1->setSaveImageDirName("Camera1");
	m_camera_thread1->setAalConfigureName("Camera1");
	m_camera_thread1->setSaveImageNum(50);
	connect(m_camera_thread1, SIGNAL(signal_image(void*)), this, SLOT(receiveLeftImage(void*)));
	connect(m_camera_thread1, SIGNAL(signal_error(QString)), this, SLOT(receiveError(QString)));
	connect(m_camera_thread1, SIGNAL(finished()), m_camera_thread1, SLOT(deleteLater()));
	m_camera_thread1->start();*/

	m_camera_thread2 = new Camera_Thread(Camera_Thread::ConnectionType::GigEVision, LineCameraId_Dalsa2, this);
	m_camera_thread2->setSaveDatePath(value.toString());
	m_camera_thread2->setSaveImageDirName("Camera3");
	m_camera_thread2->setAalConfigureName("Camera3");
	m_camera_thread2->setSaveImageNum(50);
	connect(m_camera_thread2, SIGNAL(signal_image(void*)), this, SLOT(receiveMiddleImage(void*)));
	connect(m_camera_thread2, SIGNAL(signal_error(QString)), this, SLOT(receiveError(QString)));
	connect(m_camera_thread2, SIGNAL(finished()), m_camera_thread2, SLOT(deleteLater()));
	m_camera_thread2->start();

	/*-----Pylon Version------------*/
	m_Pylon_camera_thread1 = new PylonCamera_Thread(PylonCamera_Thread::ConnectionType::GigEVision, LineCameraId_Pylon_Basler1, this);
	m_Pylon_camera_thread1->setSaveDatePath(value.toString());
	m_Pylon_camera_thread1->setSaveImageDirName("Camera1");
	m_Pylon_camera_thread1->setAalConfigureName("Camera1");
	m_Pylon_camera_thread1->setSaveImageNum(50);
	connect(m_Pylon_camera_thread1, SIGNAL(signal_image(void*)), this, SLOT(receiveLeftImage(void*)));
	connect(m_Pylon_camera_thread1, SIGNAL(signal_error(QString)), this, SLOT(receiveError(QString)));
	connect(m_Pylon_camera_thread1, SIGNAL(finished()), m_Pylon_camera_thread1, SLOT(deleteLater()));
	m_Pylon_camera_thread1->start();

	m_Pylon_camera_thread2 = new PylonCamera_Thread(PylonCamera_Thread::ConnectionType::GigEVision, LineCameraId_Pylon_Basler2, this);
	m_Pylon_camera_thread2->setSaveDatePath(value.toString());
	m_Pylon_camera_thread2->setSaveImageDirName("Camera2");
	m_Pylon_camera_thread2->setAalConfigureName("Camera2");
	m_Pylon_camera_thread2->setSaveImageNum(50);
	connect(m_Pylon_camera_thread2, SIGNAL(signal_image(void*)), this, SLOT(receiveRightImage(void*)));
	connect(m_Pylon_camera_thread2, SIGNAL(signal_error(QString)), this, SLOT(receiveError(QString)));
	connect(m_Pylon_camera_thread2, SIGNAL(finished()), m_Pylon_camera_thread2, SLOT(deleteLater()));
	m_Pylon_camera_thread2->start();

}

void hello::HandleImageThread(HImage& ima, LocationView view)
{
	HandlePicThread* m_pHandlePicThread = new HandlePicThread(this);
	m_pHandlePicThread->m_Image = ima;
	m_pHandlePicThread->m_WindowHandle = GetViewWindowHandle(view);

	connect(m_pHandlePicThread, SIGNAL(resultReady(bool)), this, SLOT(handleResults(bool)));
	connect(m_pHandlePicThread, SIGNAL(finished()), m_pHandlePicThread, SLOT(deleteLater()));
	m_pHandlePicThread->start();
}