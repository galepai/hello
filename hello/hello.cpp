#include "hello.h"
#include <qfiledialog.h>
#include <QMessageBox>
#include <qthread.h>
#include "CHH.h"
#include "aboutdialog.h"
#include "configuredlg.h"
#include "ComDialog.h"
#include "DeltaThread.h"


hello::hello(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	m_LeftWindowHandle = m_MiddleWindowHandle = m_RightWindowHandle = 0;
	
	QObject::connect(ui.About, &QAction::triggered, this, &hello::About);
	QObject::connect(ui.Open, &QAction::triggered, this, &hello::Open);
	QObject::connect(ui.OnRun, &QAction::triggered, this, &hello::click);
	QObject::connect(ui.Configure, &QAction::triggered, this, &hello::OnLineRun);
	QObject::connect(ui.Configure2, SIGNAL(triggered()), this, SLOT(OnConfigure2()));
	QObject::connect(ui.ShutDown, SIGNAL(triggered()), this, SLOT(OnShutDown()));

	WriteCurrenDateTime("config.ini", "Config", "OpenProgramTime");  //不支持中文写入配置文件

	qDebug() << "Hello  Thread : " << QThread::currentThreadId();
	
}


hello::~hello()
{

}

void hello::About()
{
	AboutDialog Dlg(this);
	Dlg.exec();
}


//创建线程
void hello::click()
{
	//HandlePicThread* m_pHandlePicThread = new HandlePicThread(this);
	//m_pHandlePicThread->m_Image = m_Image;
	//m_pHandlePicThread->m_WindowHandle = GetViewWindowHandle(LeftView);

	//connect(m_pHandlePicThread, SIGNAL(resultReady(bool)), this, SLOT(handleResults(bool)));
	//// 线程结束后，自动销毁
	//connect(m_pHandlePicThread, SIGNAL(finished()), m_pHandlePicThread, SLOT(deleteLater()));
	//m_pHandlePicThread->start();

	Delta_Thread::StopRun(true);

}

void hello::OnShutDown()
{
	//close();
	std::string str1 = ":000505FFFEF6\r\n";
	std::vector<short> nums = Parse_Delta_Ascii(str1);

	Delta_Thread::AddSecondQueueInfo("00", "05", "0500", "FF00");
	Delta_Thread::AddSecondQueueInfo("00", "05", "0501", "FF00");
	Delta_Thread::AddSecondQueueInfo("00", "05", "0502", "FF00");
	
}

void hello::OnConfigure2()
{
	//ConfigureDlg Dlg(this);
	//Dlg.exec();

	ComDialog Dlg(this);
	Dlg.exec();

	QVariant value;
	ReadConfigure("config.ini", "Port", "Port", value);
	QString PortName = value.toString();
	ReadConfigure("config.ini", "Port", "Baud", value);
	int Baud = value.toInt();
	ReadConfigure("config.ini", "Port", "DataBits", value);
	int DataBits = value.toInt();

	if (Delta_Thread::GetSerialPort() == nullptr)
	{
		Delta_Thread* thread = new Delta_Thread;
		thread->InitSerialPortInfo(PortName.toStdString().c_str(), Baud, QSerialPort::Parity::EvenParity, QSerialPort::DataBits(DataBits));
		connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
		connect(thread, SIGNAL(emitdata(QByteArray)), this, SLOT(readyDataSlot(QByteArray)));  //接收数据主界面类
		thread->start();
	}
	
}

//得到线程的信号
void hello::handleResults(bool is_bad)
{
	if (m_RightWindowHandle != 0)
		m_bIsBad = is_bad;	
}

//创建线程，串口多线程接收数据
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
	qDebug() << "MainSlot Thread : " << QThread::currentThreadId();
	//statusBar()->showMessage(str.toHex());
	statusBar()->showMessage(str);	

}


void hello::Open()
{

	QString path = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files(*.jpg *.png *.bmp)"));
	
	if (path.length() != 0 && 
		(path.contains(".bmp") || path.contains(".jpg"))) 
	{
		ReadImage(&m_Image, path.toLocal8Bit().constData());
		DispPic(m_Image,LeftView);
		WriteConfigure("0001.all", "Pic_Path", "GB_Pic", path);
	}
}

//void hello::Open()
//{
//	QString path = QFileDialog::getOpenFileName(this, tr("Open ALL"), "", tr("Image Files(*.gll)"));
//
//	//ReadConfigure("")
//	if (path.length() != 0 &&
//		(path.contains(".bmp") || path.contains(".jpg")))
//	{
//		ReadImage(&m_Image, path.toLocal8Bit().constData());
//		DispPic(m_Image,LeftView);
//	}
//}


void hello::DispPic(HImage& Image, LocationView location)
{
	int width = Image.Width();
	int height = Image.Height();
	HTuple* pWindowHandle = nullptr;
	if (location == LeftView)
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
	}

	if (*pWindowHandle == 0)
	{
		if (location == RightView)
		{
			CHH::dev_open_window_fit_image(Image, 0, 0, 1024, 4000, (long)ui.RightPicView->winId(), pWindowHandle);
		}
		else if (location == MiddleView)
		{
			CHH::dev_open_window_fit_image(Image, 0, 0, 1024, 4000, (long)ui.RightPicView->winId(), pWindowHandle);
		}
		else
		{
			CHH::dev_open_window_fit_image(Image, 0, 0, 1024, 4000, (long)ui.LeftPicView->winId(), pWindowHandle);
		}
		
		DispObj(Image, *pWindowHandle);
		float scaleX = width / 1024.0;
		float scaleY = height / 4000.0;
		if (scaleY > scaleX)
		{
			HSCROLL_HEIGHT_RightPic(4000);
		}
		else
		{
			HSCROLL_HEIGHT_RightPic(500);
		}
	}
	else
	{
		ClearWindow(*pWindowHandle);
		CloseWindow(*pWindowHandle);

		if (location == RightView)
		{
			CHH::dev_open_window_fit_image(Image, 0, 0, 1024, 4000, (long)ui.RightPicView->winId(), pWindowHandle);
		}
		else if (location == MiddleView)
		{
			CHH::dev_open_window_fit_image(Image, 0, 0, 1024, 4000, (long)ui.RightPicView->winId(), pWindowHandle);
		}
		else
		{
			CHH::dev_open_window_fit_image(Image, 0, 0, 1024, 4000, (long)ui.LeftPicView->winId(), pWindowHandle);
		}

		DispImage(Image, *pWindowHandle);
		float scaleX = width / 1024.0;
		float scaleY = height / 4000.0;
		if (scaleY > scaleX)
		{
			HSCROLL_HEIGHT_RightPic(4000);
		}
		else
		{
			HSCROLL_HEIGHT_RightPic(500);
		}
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
