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



hello::hello(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	m_LeftWindowHandle = m_MiddleWindowHandle = m_RightWindowHandle = 0;
	// �˵�����Ӧ����
	connect(ui.About, &QAction::triggered, this, &hello::OnAbout);
	connect(ui.OnLRC, &QAction::triggered, this, &hello::OnLRC);
	//��������Ӧ����
	connect(ui.Open, &QAction::triggered, this, &hello::OnOpen);
	connect(ui.OnRun, &QAction::triggered, this, &hello::OnOneHandle);
	connect(ui.OnLineRun, SIGNAL(triggered()), this, SLOT(OnLineRun()));
	connect(ui.OnStop, SIGNAL(triggered()), this, SLOT(OnStop()));
	connect(ui.Configure, SIGNAL(triggered()), this, SLOT(OnConfigure()));
	connect(ui.ShutDown, SIGNAL(triggered()), this, SLOT(OnShutDown()));
	connect(ui.ShutDown, SIGNAL(triggered()), this, SLOT(OnShutDown()));
	connect(ui.Configure2, SIGNAL(triggered()), this, SLOT(OnTest()));
	//�Ҷ˽����Ӧ����
	connect(ui.btn_debug, SIGNAL(clicked()), this, SLOT(DebugDialog()));
	connect(ui.btn_start, SIGNAL(clicked()), this, SLOT(OnStart()));
	connect(ui.btn_stop, SIGNAL(clicked()), this, SLOT(OnStop()));
	connect(ui.verticalSlider_mode, SIGNAL(valueChanged(int)), this, SLOT(ChangeMode(int)));

	WriteCurrenDateTime("config.ini", "Config", "OpenProgramTime");  //��֧������д�������ļ�
	
	SetRightTableView();
	
	//��ʱˢ���б���������ײ�
	QTimer* timer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(TableSrolltoBottom()));
	timer->start(100);
	
	//HIDDLE_DIALOG_BUTTON
	FullScreenShow();	//ȫ����ʾ

	m_camera_thread1 = nullptr;
	m_camera_thread2 = nullptr;

}

//ȫ����ʾ
void hello::FullScreenShow()
{
	QRect rect = qApp->desktop()->geometry();
	setGeometry(qApp->desktop()->geometry());
	setContentsMargins(0, 0, 0, 0);
}

//��������ͼ����
void hello::SetRightTableView()
{
	Right_dataModel = new QStandardItemModel();
	ui.tableView_right->setModel(Right_dataModel);  //������ģ��
	ui.tableView_right->setShowGrid(false);

	//�����б�ͷ
	QStringList labels = (G2U("ʱ��,������,��������,�����¼�,��ǰֵ,����ֵ,��������")).simplified().split(",");
	Right_dataModel->setHorizontalHeaderLabels(labels);

	ui.tableView_right->setColumnWidth(0, 150);
	ui.tableView_right->setColumnWidth(1, 80);
	ui.tableView_right->setColumnWidth(2, 80);
	ui.tableView_right->setColumnWidth(3, 80);
	ui.tableView_right->setColumnWidth(4, 80);
	ui.tableView_right->setColumnWidth(5, 80);
}

//�б���������ײ�
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

//�����߳�
void hello::OnOneHandle()
{
	HandlePicThread* m_pHandlePicThread = new HandlePicThread(this);
	m_pHandlePicThread->m_Image = m_Image;
	m_pHandlePicThread->m_WindowHandle = GetViewWindowHandle(RightView);

	connect(m_pHandlePicThread, SIGNAL(resultReady(bool)), this, SLOT(handleResults(bool)));
	// �߳̽������Զ�����
	connect(m_pHandlePicThread, SIGNAL(finished()), m_pHandlePicThread, SLOT(deleteLater()));
	m_pHandlePicThread->start();
}

void hello::OnShutDown()
{
	if (Camera_Thread::IsExistCameraId(AreaCameraId2))
	{
		if (m_camera_thread1->isRunning())
		{
			m_camera_thread1->stop();
			m_camera_thread1->wait();
		}
	}

	if (Camera_Thread::IsExistCameraId(LineCameraId))
	{
		if (m_camera_thread2->isRunning())
		{
			m_camera_thread2->stop();
			m_camera_thread2->wait();
		}
	}

	//m_camera_thread1->destroyed();
	//m_camera_thread1->exit();
	//close();
	qApp->quit();
}

void hello::OnConfigure()
{
	ConfigureDlg Dlg(this);
	Dlg.exec();
}

//�õ��̵߳��ź�
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

//��Ӧ���ڵĶ�ȡ�ۺ���
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
			new QStandardItem(G2U("���޾���")),
			new QStandardItem(G2U("��������")),
			QColor(0, 0, 0));
	}
	
}

//�����Ϣ�����Զ�����
void hello::AddItemToTableView(QTableView* view, QStandardItemModel* model, QStandardItem* item0, QStandardItem* item1, QStandardItem* item2, QStandardItem* item3, const QBrush& textcolor, int RowHeight)
{
	int Row = model->rowCount();

	model->setItem(Row, 0, item0);
	model->item(Row, 0)->setTextAlignment(Qt::AlignCenter);	//�������ݾ�����ʾ
	model->item(Row, 0)->setForeground(textcolor); //����������ʾ��ɫ

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
	
	if (path.length() != 0 && 
		(path.contains(".bmp") || path.contains(".jpg"))) 
	{
		ReadImage(&m_Image, path.toLocal8Bit().constData());
		DispPic(m_Image,RightView);
		WriteConfigure("0001.all", "Pic_Path", "GB_Pic", path);
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
		ClearWindow(*pWindowHandle);
		CloseWindow(*pWindowHandle);

		SetOpenWindowHandle(Image, pWindowHandle, location);
		DispObj(Image, *pWindowHandle);
		SetPicViewScroll(width, height, location);
	}

}

//ָ����ʾ���ڵľ��
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

//ָ����ͼ�Ĺ���������
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

/***********�Ҷ˵��Թ���************/
//�򿪵��Խ���
void hello::DebugDialog()
{
	TestDialog dlg(this);
	dlg.exec();
}

//����
void hello::OnStart()
{
	ui.btn_start->setEnabled(false);
	QVariant value;
	ReadConfigure("config.ini", "Port", "Port", value);
	QString PortName = value.toString();
	ReadConfigure("config.ini", "Port", "Baud", value);
	int Baud = value.toInt();
	ReadConfigure("config.ini", "Port", "DataBits", value);
	int DataBits = value.toInt();

	//����Ĭ�ϲ�ѯ����
	Delta_Thread::setQueryMode(Delta_Thread::QueryMode::DefalutQuene);
	Delta_Thread::AddDefaultQueueInfo("00050500FF00");
	Delta_Thread::AddDefaultQueueInfo("00050501FF00");
	Delta_Thread::AddDefaultQueueInfo("00050502FF00");
	Delta_Thread::AddDefaultQueueInfo("00050503FF00");
	Delta_Thread::AddDefaultQueueInfo("00050504FF00");

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
}

//ֹͣ
void hello::OnStop()
{
	Delta_Thread::StopRun(true);

	ui.btn_start->setEnabled(true);
	ui.btn_stop->setEnabled(false);
	ui.verticalSlider_mode->setEnabled(true);
}

//ģʽ�л�
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
}

void hello::receiveRightImage(void* image)
{
	HImage ima = *(HImage*)image;
	DispPic(ima, RightView);
}

void hello::receiveMiddleImage(void* image)
{
	HImage ima = *(HImage*)image;
	DispPic(ima, MiddleView);
}

void hello::receiveError(QString error)
{
	QMessageBox::StandardButton reply;
	reply = QMessageBox::warning(this, G2U("����"), error);
}

void hello::OnTest()
{
	m_camera_thread1 = new Camera_Thread(Camera_Thread::ConnectionType::DirectShow, AreaCameraId2, this);
	QVariant value;
	ReadConfigure("config.ini", "Config", "ImagePath3", value);
	m_camera_thread1->setSaveImagePath(value.toString());
	m_camera_thread1->setSaveImageNum(50);
	connect(m_camera_thread1, SIGNAL(signal_image(void*)), this, SLOT(receiveLeftImage(void*)));
	connect(m_camera_thread1, SIGNAL(signal_error(QString)), this, SLOT(receiveError(QString)));
	connect(m_camera_thread1, SIGNAL(finished()), m_camera_thread1, SLOT(deleteLater()));
	m_camera_thread1->start();

	/*m_camera_thread2 = new Camera_Thread(Camera_Thread::ConnectionType::GigEVision, LineCameraId, this);
	ReadConfigure("config.ini", "Config", "ImagePath4", value);
	m_camera_thread2->setSaveImagePath(value.toString());
	m_camera_thread1->setSaveImageNum(50);
	connect(m_camera_thread2, SIGNAL(signal_image(void*)), this, SLOT(receiveRightImage(void*)));
	connect(m_camera_thread2, SIGNAL(signal_error(QString)), this, SLOT(receiveError(QString)));
	connect(m_camera_thread2, SIGNAL(finished()), m_camera_thread2, SLOT(deleteLater()));
	m_camera_thread2->start();*/
}