#include "hello.h"
#include "CHH.h"
#include "aboutdialog.h"
#include <qfiledialog.h>
#include <qdirmodel.h>
#include <qfilesystemmodel.h>
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
	
	qDebug() << "Hello Thread : " << QThread::currentThreadId();


	//QSettings setting("config.ini", QSettings::IniFormat);//生成配置文件
	//setting.beginGroup("config");//beginGroup与下面endGroup 相对应，“config”是标记
	//setting.setValue("page", QVariant(12345));
	//setting.setValue("site", QVariant("http://www.cppblog.com/gaimor/"));
	//setting.setValue("maker", QVariant("Gaimor"));
	//setting.endGroup();
	//setting.beginGroup("config2");//beginGroup与下面endGroup 相对应，“config”是标记
	//setting.setValue("page", QVariant(6789));
	//setting.setValue("site", QVariant("http://www.cppblog.com/gaimor/"));
	//setting.setValue("maker", QVariant("tester"));
	//setting.endGroup();


	WriteCurrenDateTime("config.ini", "Config", "Time");  //好像不支持中文写入配置文件

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

	//pCommThread->SetPauseThread(true);


}

//得到线程的信号
void hello::handleResults(bool is_bad)
{
	if (m_WindowHandle != 0)
		m_bIsBad = is_bad;
		
}

//创建线程
void hello::OnLineRun()
{
	QThread *thread = new QThread;
	MyObject* my = new MyObject;
	pobject = (MyObject*)my;
	
	((MyObject*)pobject)->pHello = this;
	((MyObject*)pobject)->setPortName("com3");
	((MyObject*)pobject)->setBaudRate(9600);

	((MyObject*)pobject)->moveToThread(thread);
	thread->start();
	//connect(thread, SIGNAL(started()), my, SLOT(first()));
	
	if (((MyObject*)pobject)->open())
	{
		//connect(pCommThread, SIGNAL(resultReady(bool)), this, SLOT(handleResults(bool)));
		QObject::connect(((MyObject*)pobject)->GetSerialPort(), SIGNAL(readyRead()), ((MyObject*)pobject), SLOT(recivedata()));  //串口读取到数据信号，响应槽函数
		QObject::connect(((MyObject*)pobject), SIGNAL(emitdata(QByteArray)), this, SLOT(readyDataSlot1(QByteArray)));  //串口读取到数据信号，响应槽函数
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


void hello::readyDataSlot1(QByteArray str)  //对应串口的读取槽函数
{
	//QByteArray requestData;
	//char temp[8] = "";
	////char* temp = new char[8];
	//memset(temp, 0, 8);
	////requestData = pCommThread->GetSerialPort()->readAll();
	////requestData = pCommThread->GetSerialPort()->read(8);
	//pCommThread->readData(temp, 8);
	//requestData.resize(8);

	//for (int i = 0; i<8; i++) {
	//	requestData[i] = temp[i];
	//}
	////if (!requestData.isEmpty())
	////{
	qDebug() << "readyDataSlot1 Thread : " << QThread::currentThreadId();
	statusBar()->showMessage(str.toHex());
	////}
	//requestData.clear();
}

void hello::readyDataSlot()  //对应串口的读取槽函数
{
	//QByteArray requestData;
	//char temp[8] = "";
	////char* temp = new char[8];
	//memset(temp, 0, 8);
	////requestData = pCommThread->GetSerialPort()->readAll();
	////requestData = pCommThread->GetSerialPort()->read(8);
	//pCommThread->readData(temp, 8);
	//requestData.resize(8);

	//for (int i = 0; i<8; i++) {
	//	requestData[i] = temp[i];
	//}
	////if (!requestData.isEmpty())
	////{
	//statusBar()->showMessage(requestData.toHex());
	////}
	//requestData.clear();
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


