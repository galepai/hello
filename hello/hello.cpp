#include "hello.h"
#include "CHH.h"
#include "aboutdialog.h"
#include "configuredlg.h"
#include <qfiledialog.h>
#include <qdirmodel.h>
#include <qfilesystemmodel.h>
#include <QMessageBox>
#include <qthread.h>
#include "Func.h"
#include "CommThread.h"
#include "SerialPortThread.h"

hello::hello(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	m_Title = windowTitle();
	m_WindowHandle = 0;
	
	QObject::connect(ui.About, &QAction::triggered, this, &hello::About);
	QObject::connect(ui.Open, &QAction::triggered, this, &hello::Open);
	QObject::connect(ui.OnRun, &QAction::triggered, this, &hello::click);
	QObject::connect(ui.Configure, &QAction::triggered, this, &hello::OnLineRun);
	QObject::connect(ui.Configure2, SIGNAL(triggered()), this, SLOT(OnConfigure2()));
	QObject::connect(ui.ShutDown, SIGNAL(triggered()), this, SLOT(OnShutDown()));
	qDebug() << "Hello Thread : " << QThread::currentThreadId();

	WriteCurrenDateTime("config.ini", "Config", "OpenProgramTime");  //好像不支持中文写入配置文件

	qDebug() << "hello";


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
	HandlePicThread* m_pHandlePicThread = new HandlePicThread(this);
	m_pHandlePicThread->m_Image = m_Image;
	m_pHandlePicThread->m_WindowHandle = m_WindowHandle;

	connect(m_pHandlePicThread, SIGNAL(resultReady(bool)), this, SLOT(handleResults(bool)));
	// 线程结束后，自动销毁
	connect(m_pHandlePicThread, SIGNAL(finished()), m_pHandlePicThread, SLOT(deleteLater()));
	m_pHandlePicThread->start();

}

void hello::OnShutDown()
{
	close();
}

void hello::OnConfigure2()
{
	ConfigureDlg Dlg(this);
	Dlg.exec();
}

//得到线程的信号
void hello::handleResults(bool is_bad)
{
	if (m_WindowHandle != 0)
		m_bIsBad = is_bad;	
}

//创建线程
void hello::OnLineRun()
{//串口多线程接收数据
	QVariant Value;
	ReadConfigure("config.ini", "Port", "Port", Value);
	QString port = Value.toString();
	ReadConfigure("config.ini", "Port", "Baud", Value);
	int baud = Value.toInt();

	statusBar()->showMessage(port + "," + Value.toString());

	QThread *thread = new QThread;
	SerialPort* pSerialPort = new SerialPort;

	pSerialPort->setPortName(port);
	pSerialPort->setBaudRate(baud);
	pSerialPort->setReciveLen(8);

	pSerialPort->moveToThread(thread);
	thread->start();
	
	if (pSerialPort->open())
	{
		QObject::connect(pSerialPort->GetSerialPort(), SIGNAL(readyRead()), pSerialPort, SLOT(recivedata()));  //串口读取到数据信号，响应槽函数，多线程
		QObject::connect(pSerialPort, SIGNAL(emitdata(QByteArray)), this, SLOT(readyDataSlot(QByteArray)));  //接收数据主界面类
	}
	else
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::warning(this, G2U("打开串口错误"), pSerialPort->GetSerialPort()->errorString());

		delete pSerialPort;
		pSerialPort = nullptr;
	}

	////////////////
	//CommThread* pCommThread = new CommThread(nullptr);
	//pCommThread->pHello = this;
	//pCommThread->setPortName("com3");
	//pCommThread->setBaudRate(9600);
	//
	//if (pCommThread->open())
	//{
	//	//connect(pCommThread, SIGNAL(resultReady(bool)), this, SLOT(handleResults(bool)));
	//	//QObject::connect(pCommThread->GetSerialPort(), SIGNAL(readyRead()), pCommThread, SLOT(recivedata()));  //串口读取到数据信号，响应槽函数
	//	// 线程结束后，自动销毁
	//	//connect(pCommThread, SIGNAL(finished()), pCommThread, SLOT(deleteLater()));
	//	pCommThread->start();
	//}	
	/*else
	{
		QString error = pCommThread->GetSerialPort()->errorString();
		delete pCommThread;
		pCommThread = nullptr;
	}*/
}


void hello::readyDataSlot(QByteArray str)  //对应串口的读取槽函数
{
	qDebug() << "MainSlot Thread : " << QThread::currentThreadId();
	statusBar()->showMessage(str.toHex());
}


void hello::Open()
{
	QString path = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files(*.jpg *.png *.bmp)"));
	
	if (path.length() != 0 && 
		(path.contains(".bmp") || path.contains(".jpg"))) 
	{
		ReadImage(&m_Image, path.toLocal8Bit().constData());
		DispPic(m_Image);	
	}
}


void hello::DispPic(HImage& Image)
{
	int width = Image.Width();
	int height = Image.Height();


	if (m_WindowHandle == 0)
	{
		CHH::dev_open_window_fit_image(Image, 0, 0, 1024, 4000, (long)ui.RightPicView->winId(), &m_WindowHandle);
		DispObj(Image, m_WindowHandle);
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
		ClearWindow(m_WindowHandle);
		CloseWindow(m_WindowHandle);

		CHH::dev_open_window_fit_image(Image, 0, 0, 1024, 4000, (long)ui.RightPicView->winId(), &m_WindowHandle);
		DispImage(Image, m_WindowHandle);
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


