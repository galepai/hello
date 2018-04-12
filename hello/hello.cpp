#include "hello.h"
#include <qfiledialog.h>
#include <QMessageBox>
#include <QThread>
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

	connect(ui.btn_debug, SIGNAL(clicked()), this, SLOT(DebugDialog()));
	connect(ui.btn_start, SIGNAL(clicked()), this, SLOT(start()));
	connect(ui.btn_stop, SIGNAL(clicked()), this, SLOT(stop()));
	connect(ui.verticalSlider_mode, SIGNAL(valueChanged(int)), this, SLOT(ChangeMode(int)));

	pDlg = new CommDialog(this);
	pDlg->setModal(false);

	WriteCurrenDateTime("config.ini", "Config", "OpenProgramTime");  //不支持中文写入配置文件

	qDebug() << "Hello  Thread : " << QThread::currentThreadId();

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
	
	QDateTime curTime;

	for (int i = 0; i < 100; i++)
	{
		AddItemToTableView(ui.tableView_right, Right_dataModel,
			new QStandardItem(curTime.currentDateTime().toString("yyyy-MM-dd hh:mm:ss")),
			new QStandardItem(QString("Data%1").arg(i)),
			new QStandardItem(G2U("上限警报")),
			new QStandardItem(G2U("报警结束")),
			QColor(0, 0, 0));
	}

	
	QTimer* timer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(TableSrolltoBottom()));
	timer->start(100);
	
}

//添加信息至表，自动增行
void hello::AddItemToTableView(QTableView* view, QStandardItemModel* model, QStandardItem* item0, QStandardItem* item1, QStandardItem* item2, QStandardItem* item3, const QBrush& textcolor, int RowHeight)
{
	int Row = model->rowCount();
	model->setItem(Row, 0, item0);
	model->item(Row, 0)->setTextAlignment(Qt::AlignCenter);	//设置数据居中显示
	model->item(Row, 0)->setForeground(textcolor); //设置数据显示颜色

	model->setItem(Row, 1, item1);
	model->item(Row, 1)->setTextAlignment(Qt::AlignCenter);	//设置数据居中显示
	model->item(Row, 1)->setForeground(textcolor); //设置数据显示颜色

	model->setItem(Row, 2, item2);
	model->item(Row, 2)->setTextAlignment(Qt::AlignCenter);	//设置数据居中显示
	model->item(Row, 2)->setForeground(textcolor); //设置数据显示颜色

	model->setItem(Row, 3, item3);
	model->item(Row, 3)->setTextAlignment(Qt::AlignCenter);	//设置数据居中显示
	model->item(Row, 3)->setForeground(textcolor); //设置数据显示颜色

	view->setRowHeight(Row, RowHeight);
	//model->sort(0, Qt::AscendingOrder);

}
void hello::TableSrolltoBottom()
{
	/*if (Right_dataModel->rowCount() > 50)
	{
		QStandardItem* item[4];
		item[0] = Right_dataModel->item(0, 0);
		item[1] = Right_dataModel->item(0, 1);
		item[2] = Right_dataModel->item(0, 2);
		item[3] = Right_dataModel->item(0, 3);

		delete []item;
		Right_dataModel->removeRow(0);
	}*/

	if (!ui.tableView_right->hasFocus())
		ui.tableView_right->scrollToBottom();

}

hello::~hello()
{

}

void hello::About()
{
	//AboutDialog Dlg(this);
	//Dlg.exec();
	pDlg->show();
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
	//qDebug() << "MainSlot Thread : " << QThread::currentThreadId();
	statusBar()->showMessage(str);	
	//QDateTime curTime;
	//AddItemToTableView(ui.tableView_right, Right_dataModel,
	//	new QStandardItem(curTime.currentDateTime().toString("yyyy-MM-dd hh:mm:ss")),
	//	new QStandardItem(QString(str)),
	//	new QStandardItem(G2U("上限警报")),
	//	new QStandardItem(G2U("报警结束")),
	//	QColor(0, 0, 0));
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

/***********右端调试功能************/
//打开调试界面
void hello::DebugDialog()
{
	TestDialog dlg(this);
	dlg.exec();
}

//启动
void hello::start()
{
	ui.btn_start->setEnabled(false);
	QVariant value;
	ReadConfigure("config.ini", "Port", "Port", value);
	QString PortName = value.toString();
	ReadConfigure("config.ini", "Port", "Baud", value);
	int Baud = value.toInt();
	ReadConfigure("config.ini", "Port", "DataBits", value);
	int DataBits = value.toInt();

	Delta_Thread::setQueryMode(DefalutQuene);

	Delta_Thread::AddDefaultQueueInfo("00050500FF00");
	Delta_Thread::AddDefaultQueueInfo("00050501FF00");
	Delta_Thread::AddDefaultQueueInfo("00050502FF00");
	Delta_Thread::AddDefaultQueueInfo("00050503FF00");
	Delta_Thread::AddDefaultQueueInfo("00050504FF00");

	if (Delta_Thread::GetSerialPort() == nullptr)
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
}

//停止
void hello::stop()
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