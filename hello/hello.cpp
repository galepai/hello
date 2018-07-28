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
#include "PicThreadMiddle.h"
#include "PicThreadLeft.h"
#include "PicThreadRight.h"



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
	connect(ui.btn_detect_end, SIGNAL(clicked()), this, SLOT(OnDetectEnd()));
	connect(ui.verticalSlider_mode, SIGNAL(valueChanged(int)), this, SLOT(ChangeMode(int)));
	ui.btn_debug->setEnabled(false);
	ui.btn_stop->setEnabled(false);

	WriteCurrenDateTime("config.ini", "Config", "OpenProgramTime");  //不支持中文写入配置文件
	
	
	//HIDDLE_DIALOG_BUTTON
	FullScreenShow();	//全屏显示

	m_camera_thread1 = nullptr;
	m_camera_thread2 = nullptr;
	m_Pylon_camera_thread1 = nullptr;
	m_Pylon_camera_thread2 = nullptr;

	m_AllResult = 0;
	
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
	PicThreadMiddle* m_pHandlePicThread = new PicThreadMiddle(this);
	m_pHandlePicThread->m_Image = m_Image;
	m_pHandlePicThread->m_WindowHandle = GetViewWindowHandle(RightView);

	connect(m_pHandlePicThread, SIGNAL(resultReady(bool)), this, SLOT(handleResults(bool)));
	// 线程结束后，自动销毁
	connect(m_pHandlePicThread, SIGNAL(finished()), m_pHandlePicThread, SLOT(deleteLater()));
	m_pHandlePicThread->start();
}

void hello::OnShutDown()
{
	if (!ui.btn_stop->isEnabled())
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::question(this, G2U("系统"), G2U("确认退出系统"));
		if (reply == QMessageBox::StandardButton::Yes)
		{
			//OnClearCameraThread();
			qApp->quit();
		}
	}
	else
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::information(this, G2U("提示"), G2U("请先停止系统,请点击右侧的'停止''按钮"));

	}
	
}

void hello::OnClearCameraThread()
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
}

void hello::OnConfigure()
{
	/*ConfigureDlg Dlg(this);
	Dlg.exec();*/

	ConfigureDialog Dlg(this);
	Dlg.exec();
}

void hello::OnLineRun()
{
	QVariant Value;
	ReadConfigure("config.ini", "Port", "Port", Value);
	QString port = Value.toString();
	ReadConfigure("config.ini", "Port", "Baud", Value);
	int baud = Value.toInt();
	statusBar()->showMessage(port + "," + Value.toString());

	//OnTest();


}

//对应串口的读取槽函数
void hello::readyDataSlot(QByteArray str)  
{	
	//qDebug() << "MainSlot Thread : " << QThread::currentThreadId();
	statusBar()->showMessage(str);	
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
{
	if (OpenSerial())
	{

		//QVariant value;
		//ReadConfigure("config.ini", "Config", "ImagePath1", value);


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
		connect(m_camera_thread1, SIGNAL(grab_correct_image(int)), this, SLOT(receiveCorrectImage(int)));
		connect(m_camera_thread1, SIGNAL(finished()), m_camera_thread1, SLOT(deleteLater()));
		m_camera_thread1->start();*/

		m_camera_thread2 = new Camera_Thread(Camera_Thread::ConnectionType::GigEVision, LineCameraId_Dalsa2, this);
		//m_camera_thread2->setSaveDatePath(value.toString());
		m_camera_thread2->setSaveImageDirName("Camera3");
		m_camera_thread2->setAalConfigureName("Camera3");
		m_camera_thread2->setSaveImageNum(50);
		connect(m_camera_thread2, SIGNAL(signal_image(void*)), this, SLOT(receiveMiddleImage(void*)));
		connect(m_camera_thread2, SIGNAL(signal_error(QString)), this, SLOT(receiveError(QString)));
		connect(m_camera_thread2, SIGNAL(grab_correct_image(int)), this, SLOT(receiveCorrectImage(int)));
		connect(m_camera_thread2, SIGNAL(finished()), m_camera_thread2, SLOT(deleteLater()));
		m_camera_thread2->start();

		/*-----Pylon Version------------*/
		//m_Pylon_camera_thread1 = new PylonCamera_Thread(PylonCamera_Thread::ConnectionType::GigEVision, LineCameraId_Pylon_Basler1, this);
		//m_Pylon_camera_thread1->setSaveDatePath(value.toString());
		//m_Pylon_camera_thread1->setSaveImageDirName("Camera1");
		//m_Pylon_camera_thread1->setAalConfigureName("Camera1");
		//m_Pylon_camera_thread1->setSaveImageNum(50);
		//connect(m_Pylon_camera_thread1, SIGNAL(signal_image(void*)), this, SLOT(receiveLeftImage(void*)));
		//connect(m_Pylon_camera_thread1, SIGNAL(signal_error(QString)), this, SLOT(receiveError(QString)));
		//connect(m_Pylon_camera_thread1, SIGNAL(grab_correct_image(int)), this, SLOT(receiveCorrectImage(int)));
		//connect(m_Pylon_camera_thread1, SIGNAL(finished()), m_Pylon_camera_thread1, SLOT(deleteLater()));
		//m_Pylon_camera_thread1->start();

		//m_Pylon_camera_thread2 = new PylonCamera_Thread(PylonCamera_Thread::ConnectionType::GigEVision, LineCameraId_Pylon_Basler2, this);
		////m_Pylon_camera_thread2->setSaveDatePath(value.toString());
		//m_Pylon_camera_thread2->setSaveImageDirName("Camera2");
		//m_Pylon_camera_thread2->setAalConfigureName("Camera2");
		//m_Pylon_camera_thread2->setSaveImageNum(50);
		//connect(m_Pylon_camera_thread2, SIGNAL(signal_image(void*)), this, SLOT(receiveRightImage(void*)));
		//connect(m_Pylon_camera_thread2, SIGNAL(signal_error(QString)), this, SLOT(receiveError(QString)));
		//connect(m_Pylon_camera_thread2, SIGNAL(grab_correct_image(int)), this, SLOT(receiveCorrectImage(int)));
		//connect(m_Pylon_camera_thread2, SIGNAL(finished()), m_Pylon_camera_thread2, SLOT(deleteLater()));
		//m_Pylon_camera_thread2->start();

		ui.btn_start->setEnabled(false);
		ui.btn_stop->setEnabled(true);
	}

}

//停止
void hello::OnStop()
{
	Delta_Thread::StopRun(true);

	ui.verticalSlider_mode->setEnabled(true);
	OnClearCameraThread();

	ui.btn_start->setEnabled(true);
	ui.btn_stop->setEnabled(false);
	
}

//检测完成信号
void hello::OnDetectEnd()
{
	if (Delta_Thread::GetSerialPort())
	{
		Delta_Thread::AddOneQueryInfo(DETECT_END_ON);
		Delta_Thread::AddOneQueryInfo(DETECT_END_OFF);
	}		
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
	HandleImageThread(ima, MiddleView);
}

void hello::receiveError(QString error)
{
	QMessageBox::StandardButton reply;
	reply = QMessageBox::warning(this, G2U("错误"), error);
}

//收到正确图片数,检测完成
void hello::receiveCorrectImage(int value)
{
	static int imageNum = 0;
	imageNum += value;

	if (imageNum == 1)
	{
		static int Num = 0;
		Delta_Thread::AddOneQueryInfo(DETECT_END_ON);
		Delta_Thread::AddOneQueryInfo(DETECT_END_OFF);

		imageNum = 0;
		qDebug() << "Send M177:				" << ++Num;
		ui.lineEdit_allNum->setText(QString("%1").arg(Num));
	}

}

void hello::OnTest()
{
	OnStart();
}

bool hello::OpenSerial()
{
	bool isRet = true;

	QVariant value;
	ReadConfigure("config.ini", "Port", "Port", value);
	QString PortName = value.toString();
	ReadConfigure("config.ini", "Port", "Baud", value);
	int Baud = value.toInt();
	ReadConfigure("config.ini", "Port", "DataBits", value);
	int DataBits = value.toInt();

	//Delta_Thread::setQueryMode(Delta_Thread::QueryMode::DefalutQuene);
	//Delta_Thread::AddDefaultQueueInfo(READ_X_INPUT);	//读X00-X07
	//Delta_Thread::AddDefaultQueueInfo(READ_Y_OUTPUT);	//读Y00-Y67

	//设置单次查询模式
	Delta_Thread::setQueryMode(Delta_Thread::QueryMode::OneQuery);

	if (!Delta_Thread::GetSerialPort())
	{
		Delta_Thread* thread = new Delta_Thread;
		thread->InitSerialPortInfo(PortName.toStdString().c_str(), Baud, QSerialPort::Parity::EvenParity, QSerialPort::DataBits(DataBits));
		connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
		connect(thread, SIGNAL(emitdata(QByteArray)), this, SLOT(readyDataSlot(QByteArray)));
		connect(thread, SIGNAL(error(QString)), this, SLOT(receiveError(QString)));
		connect(thread, &Delta_Thread::bool_error, [&](bool is){ isRet = is; });
		thread->start();
		Sleep(100);  //等待串口如果错误打开,返回isRet;
	}

	return isRet;

}

void hello::HandleImageThread(HImage& ima, LocationView view)
{
	switch (view)
	{
	case LeftView:
		{
			PicThreadLeft* pPicThread = new PicThreadLeft(this);
			pPicThread->m_Image = ima;
			pPicThread->m_WindowHandle = GetViewWindowHandle(view);
			connect(pPicThread, SIGNAL(resultReady(int)), this, SLOT(handleResults(int)));
			connect(pPicThread, SIGNAL(finished()), pPicThread, SLOT(deleteLater()));
			pPicThread->start();
		}
	
		break;

	case MiddleView:
		{
			PicThreadMiddle* pPicThread = new PicThreadMiddle(this);
			pPicThread->m_Image = ima;
			pPicThread->m_WindowHandle = GetViewWindowHandle(view);
			connect(pPicThread, SIGNAL(resultReady(int)), this, SLOT(handleResults(int)));
			connect(pPicThread, SIGNAL(finished()), pPicThread, SLOT(deleteLater()));
			pPicThread->start();
		}
		break;

	case RightView:
		{
			PicThreadRight* pPicThread = new PicThreadRight(this);
			pPicThread->m_Image = ima;
			pPicThread->m_WindowHandle = GetViewWindowHandle(view);
			connect(pPicThread, SIGNAL(resultReady(int)), this, SLOT(handleResults(int)));
			connect(pPicThread, SIGNAL(finished()), pPicThread, SLOT(deleteLater()));
			pPicThread->start();
		}
		break;

	/*default:
		break;*/
	}
	
}

//得到线程的信号,并判断检测结果,发送给PLC
void hello::handleResults(int singleResult)
{

	static int i = 0;
	m_AllResult += singleResult;

	/*if (m_bIsBad >= AllGood)
	{
		Delta_Thread::AddOneQueueInfo(DETECT_END_ON);
		Delta_Thread::AddOneQueueInfo(DETECT_END_OFF);

		if (m_bIsBad == AllGood)
		{
			Delta_Thread::AddOneQueueInfo(RESULT_GODD);
		}
		else
		{
			Delta_Thread::AddOneQueueInfo(RESULT_BAD);
		}

		qDebug() << "All Detect Result:	" << m_bIsBad;
		m_bIsBad = 0;

		return;
	}*/

	if (m_AllResult >= MiddleGood)
	{
		//Delta_Thread::AddOneQueryInfo(DETECT_END_ON);
		//Delta_Thread::AddOneQueryInfo(DETECT_END_OFF);
		i++;
		qDebug() << "Image num:				"<<  i;

		if (m_AllResult == MiddleGood)
		{
			Sleep(30);
			Delta_Thread::AddOneQueryInfo(RESULT_GODD);
			qDebug() << "Send Good!!!	";
		}
		else
		{
			Sleep(30);
			Delta_Thread::AddOneQueryInfo(RESULT_BAD);
			qDebug() << "Send Bad!!!	";
		}

		qDebug() << "All Detect Result:	" << m_AllResult;
		singleResult = 0;

		return;
	}

	qDebug() << "Detect is_bad:	" << singleResult;
	qDebug() << "Detect Result:	" << m_AllResult;
}
