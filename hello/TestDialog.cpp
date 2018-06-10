#include "TestDialog.h"
#include "ui_TestDialog.h"
#include "DeltaThread.h"
#include "Func.h"
#include "ConstParam.h"
#include <QPushButton>
#include <QLineEdit>
#include <QPainter>


TestDialog::TestDialog(QWidget *parent) :
    QDialog(parent),
	ui(new Ui::TestDialog)
{
    ui->setupUi(this);
	connect(ui->btn_return, SIGNAL(clicked()), this, SLOT(CloseWindow()));
	connect(ui->btn_zhuashou_left, SIGNAL(clicked()), this, SLOT(OnZhuaShouLeft()));
	connect(ui->btn_zhuashou_right, SIGNAL(clicked()), this, SLOT(OnZhuaShouRight()));
	connect(ui->btn_zhuashou_up, SIGNAL(clicked()), this, SLOT(OnZhuaShouUp()));
	connect(ui->btn_zhuashou_down, SIGNAL(clicked()), this, SLOT(OnZhuaShouDown()));
	connect(ui->btn_boshou_up, SIGNAL(clicked()), this, SLOT(OnBoShouUp()));
	connect(ui->btn_boshou_down, SIGNAL(clicked()), this, SLOT(OnBoShouDown()));
	connect(ui->btn_boshou_shuiping_up, SIGNAL(clicked()), this, SLOT(OnBoShouShiPingUp()));
	connect(ui->btn_boshou_shuiping_down, SIGNAL(clicked()), this, SLOT(OnBoShouShiPingDown()));

	connect(ui->btn_hand_open, SIGNAL(clicked()), this, SLOT(OnHandOpen()));
	connect(ui->btn_hand_close, SIGNAL(clicked()), this, SLOT(OnHandClose()));

	connect(ui->btn_vision_up, SIGNAL(clicked()), this, SLOT(OnVisionUp()));
	connect(ui->btn_vision_down, SIGNAL(clicked()), this, SLOT(OnVisionDown()));
	connect(ui->btn_rotate_start, SIGNAL(clicked()), this, SLOT(OnRotateStart()));
	connect(ui->btn_rotate_stop, SIGNAL(clicked()), this, SLOT(OnRotateStop()));

	connect(ui->btn_paoguang, SIGNAL(clicked()), this, SLOT(OnPaoGuang()));

	connect(ui->btn_manual_mode, SIGNAL(clicked()), this, SLOT(OnManualMode()));
	connect(ui->btn_auto_mode, SIGNAL(clicked()), this, SLOT(OnAutoMode()));
	connect(ui->btn_auto_start, SIGNAL(clicked()), this, SLOT(OnAutoStart()));

	connect(ui->btn_distance_confirm, SIGNAL(clicked()), this, SLOT(OnDistanceConfirm()));
	connect(ui->btn_time_confirm, SIGNAL(clicked()), this, SLOT(OnTimeConfirm()));


	QList<QPushButton *> pPushButtons = findChildren<QPushButton *>();
	for (int i = 0; i < pPushButtons.count(); i++)
	{
		pPushButtons.at(i)->setProperty("status",i>5);
	}

	m_pTimer = new QTimer(this);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(update()));
	m_pTimer->start(50);

	QVariant value;
	ReadConfigure("config.ini", "Port", "Port", value);
	QString PortName = value.toString();
	ReadConfigure("config.ini", "Port", "Baud", value);
	int Baud = value.toInt();
	ReadConfigure("config.ini", "Port", "DataBits", value);
	int DataBits = value.toInt();

	//设置默认查询队列
	Delta_Thread::setQueryMode(Delta_Thread::QueryMode::DefalutQuene);
	Delta_Thread::AddDefaultQueueInfo("000105000028");	//读Y00-Y47
	Delta_Thread::AddDefaultQueueInfo("000204000028");	//读X00-X47


	if (!Delta_Thread::GetSerialPort())
	{
		Delta_Thread* thread = new Delta_Thread;
		thread->InitSerialPortInfo(PortName.toStdString().c_str(), Baud, QSerialPort::Parity::EvenParity, QSerialPort::DataBits(DataBits));
		connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
		connect(thread, SIGNAL(emitdata(QByteArray)), this, SLOT(readyDataSlot(QByteArray)));
		thread->start();
	}
		
	Delta_Thread::AddOneQueueInfo("000310C80006");	//读D200-D205 000310C80006
	HIDDLE_DIALOG_BUTTON
}

void TestDialog::ChangeStyle()
{
	QFile qss("Resources/qss/css.qss");
	if (qss.open(QFile::ReadOnly))
	{
		qApp->setStyleSheet(qss.readAll());
		qss.close();
	}
}

void TestDialog::CloseWindow()
{
	Delta_Thread::StopRun(true);
	close();
}

void TestDialog::OnZhuaShouLeft()
{
	Delta_Thread::AddOneQueueInfo(ZHUASHOU_LEFT_ON);
	Delta_Thread::AddOneQueueInfo(ZHUASHOU_LEFT_OFF);
}

void TestDialog::OnZhuaShouRight()
{
	Delta_Thread::AddOneQueueInfo(ZHUASHOU_RIGHT_ON);	
	Delta_Thread::AddOneQueueInfo(ZHUASHOU_RIGHT_OFF);	
}

void TestDialog::OnZhuaShouUp()
{
	Delta_Thread::AddOneQueueInfo(ZHUASHOU_UP_ON);
	Delta_Thread::AddOneQueueInfo(ZHUASHOU_UP_OFF);
}

void TestDialog::OnZhuaShouDown()
{
	Delta_Thread::AddOneQueueInfo(ZHUASHOU_DOWN_ON);
	Delta_Thread::AddOneQueueInfo(ZHUASHOU_DOWN_OFF);
}

void TestDialog::OnHandOpen()
{
	Delta_Thread::AddOneQueueInfo(HAND_OPEN_ON);
	Delta_Thread::AddOneQueueInfo(HAND_OPEN_OFF);
}

void TestDialog::OnHandClose()
{
	Delta_Thread::AddOneQueueInfo(HAND_CLOSE_ON);
	Delta_Thread::AddOneQueueInfo(HAND_CLOSE_OFF);
}

void TestDialog::OnBoShouUp()
{
	Delta_Thread::AddOneQueueInfo(BOSHOU_UP_ON);
	Delta_Thread::AddOneQueueInfo(BOSHOU_UP_OFF);
}

void TestDialog::OnBoShouDown()
{
	Delta_Thread::AddOneQueueInfo(BOSHOU_DOWN_ON);
	Delta_Thread::AddOneQueueInfo(BOSHOU_DOWN_OFF);
}


void TestDialog::OnBoShouShiPingUp()
{
	Delta_Thread::AddOneQueueInfo(BOSHOU_SHUIPING_UP_ON);
	Delta_Thread::AddOneQueueInfo(BOSHOU_SHUIPING_UP_OFF);
}

void TestDialog::OnBoShouShiPingDown()
{
	Delta_Thread::AddOneQueueInfo(BOSHOU_SHUIPING_DOWN_ON);
	Delta_Thread::AddOneQueueInfo(BOSHOU_SHUIPING_DOWN_OFF);
}

void TestDialog::OnVisionUp()
{
	Delta_Thread::AddOneQueueInfo(VISION_UP_ON);
	Delta_Thread::AddOneQueueInfo(VISION_UP_OFF);
}

void TestDialog::OnVisionDown()
{
	Delta_Thread::AddOneQueueInfo(VISION_DOWN_ON);
	Delta_Thread::AddOneQueueInfo(VISION_DOWN_OFF);
}

void TestDialog::OnRotateStart()	//电平信号
{
	Delta_Thread::AddOneQueueInfo(ROTATE_START_ON);
}

void TestDialog::OnRotateStop()
{
	Delta_Thread::AddOneQueueInfo(ROTATE_STOP_ON);
	Delta_Thread::AddOneQueueInfo(ROTATE_STOP_OFF);
	Delta_Thread::AddOneQueueInfo(ROTATE_START_OFF);
}

void TestDialog::OnPaoGuang()
{
	Delta_Thread::AddOneQueueInfo(PAOGUANG_ON);
	Delta_Thread::AddOneQueueInfo(PAOGUANG_OFF);
}

void TestDialog::OnManualMode()	//手动模式
{
	Delta_Thread::AddOneQueueInfo(MANUAL_MODE);
	Delta_Thread::AddOneQueueInfo(AUTO_STOP);
	
}

void TestDialog::OnAutoMode()	//自动模式
{
	Delta_Thread::AddOneQueueInfo(AUTO_MODE);
	ui->btn_manual_mode->setEnabled(false);
}

void TestDialog::OnAutoStart()	//自动启动
{
	if (ui->btn_auto_start->text() == G2U("自动启动"))
	{
		Delta_Thread::AddOneQueueInfo(AUTO_START);
		ui->btn_auto_start->setText(G2U("停止"));	
	}
	else
	{
		Delta_Thread::AddOneQueueInfo(AUTO_STOP);
		ui->btn_auto_start->setText(G2U("自动启动"));
		ui->btn_manual_mode->setEnabled(true);
	}
}
 
void TestDialog::OnAutoStop()	//自动启动
{
	
}

void TestDialog::OnDistanceConfirm() //距离设置
{
	QString Distance = ui->lineEdit_distance_set->text();
	ushort  distance = Distance.toInt();

	std::vector<ushort> data;
	ushort first = distance / 4096;
	ushort leave = distance % 4096;
	ushort second = leave / 256;
	leave = leave % 256;
	ushort third = leave / 16;
	leave = leave % 16;
	ushort forth = leave;
	data.push_back(first);
	data.push_back(second);
	data.push_back(third);
	data.push_back(forth);
	
	QString dist("0000");
	for (int index = 0; index < data.size(); index++)
	{
		if (data[index] < 10)
			dist[index] = 0x30 + data[index];
		else
			dist[index] = 0x37 + data[index];

	}

	Delta_Thread::AddOneQueueInfo((QString(SET_DISTANCE) + dist).toStdString());

	Delta_Thread::AddOneQueueInfo("000310C80006");	//读D200-D205 000310C80006
}

void TestDialog::OnTimeConfirm()	//时间设置
{
	QString Time = ui->lineEdit_time_set->text();
	ushort  time = Time.toInt();

	std::vector<ushort> data;
	ushort first = time / 4096;
	ushort leave = time % 4096;
	ushort second = leave / 256;
	leave = leave % 256;
	ushort third = leave / 16;
	leave = leave % 16;
	ushort forth = leave;
	data.push_back(first);
	data.push_back(second);
	data.push_back(third);
	data.push_back(forth);

	QString ti("0000");
	for (int index = 0; index < data.size(); index++)
	{
		if (data[index] < 10)
			ti[index] = 0x30 + data[index];
		else
			ti[index] = 0x37 + data[index];

	}

	Delta_Thread::AddOneQueueInfo((QString(SET_TIME) + ti).toStdString());

	Delta_Thread::AddOneQueueInfo("000310C80006");	//读D200-D205 000310C80006
}

TestDialog::~TestDialog()
{
    delete ui;
	delete m_pTimer;
}

void TestDialog::setBtnQss(QPushButton *btn,
	QString normalColor, QString normalTextColor,
	QString hoverColor, QString hoverTextColor,
	QString pressedColor, QString pressedTextColor)

{
	QStringList qss;
	qss.append(QString("QPushButton{border-style:none;padding:10px;border-radius:5px;color:%1;background:%2;}").arg(normalTextColor).arg(normalColor));
	qss.append(QString("QPushButton:hover{color:%1;background:%2;}").arg(hoverTextColor).arg(hoverColor));
	qss.append(QString("QPushButton:pressed{color:%1;background:%2;}").arg(pressedTextColor).arg(pressedColor));
	btn->setStyleSheet(qss.join(""));
}

void TestDialog::setTxtQss(QLineEdit *txt, QString normalColor, QString focusColor)

{
	QStringList qss;

	qss.append(QString("QLineEdit{border:1px solid rgb(41, 57, 85);\
							border-radius: 3px;\
							background: black; \
							selection-background-color: green; \
							color: %1;\
							font-size: 14px;}")
							.arg("#ff0000"));
	qss.append(QString("QLineEdit{border-style:none;padding:6px;border-radius:5px;border:2px solid %1;}").arg(normalColor));
	qss.append(QString("QLineEdit:focus{border:2px solid %1;}").arg(focusColor));
	txt->setStyleSheet(qss.join(""));

}

void TestDialog::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	/*QList<QPushButton *> pPushButtons = findChildren<QPushButton *>();
	for (int i = 0; i < pPushButtons.count(); i++)
	{
		if (pPushButtons.at(i)->text() == G2U("拨手多位气缸缩") 
			|| pPushButtons.at(i)->text() == G2U("拨手多位气缸伸")
			|| pPushButtons.at(i)->text() == G2U("相机触发")
			|| pPushButtons.at(i)->text() == G2U("返回主界面"))
		{			
		}
		else
		{
			QPointF Center_Circle = pPushButtons.at(i)->geometry().topLeft();
			pPushButtons.at(i)->geometry().topRight();
			int width = pPushButtons.at(i)->geometry().width();
			Center_Circle.setX(Center_Circle.x() + width / 2);
			Center_Circle.setY(Center_Circle.y() - 30);

			if (pPushButtons.at(i)->property("status").toBool())
			{
				PaintCirle(painter, Center_Circle, 10, QPen(QColor(0, 255, 90), 2), QColor(35, 255, 125));
			}
			else
			{
				PaintCirle(painter, Center_Circle, 10, QPen(QColor(255, 90, 90), 2), QColor(255, 45, 35));
			}	
		}
	}*/

	int radius = 5;
	int distance = 15;
	QPointF x_origin(75, 550);
	if (m_X_States.size() == 40)
	{
		for (int row = 0; row < 5; row++)
		{
			for (int column = 0; column < 8; column++)
			{
				if (m_X_States[row * 8 + column])
					PaintCirle(painter, QPointF(x_origin.x() + column*(radius + distance),
					x_origin.y() + row*(radius + distance)),
					radius, QPen(QColor(0, 255, 90), 2), QColor(35, 255, 125));
				else
					PaintCirle(painter, QPointF(x_origin.x() + column*(radius + distance),
					x_origin.y() + row*(radius + distance)),
					radius, QPen(QColor(255, 90, 90), 2), QColor(255, 45, 35));
			}
		}
	}
	
	QPointF y_origin(350, 550);

	if (m_Y_States.size() == 40)
	{
		for (int row = 0; row < 5; row++)
		{
			for (int column = 0; column < 8; column++)
			{
				if (m_Y_States[row * 8 + column])
					PaintCirle(painter, QPointF(y_origin.x() + column*(radius + distance),
					y_origin.y() + row*(radius + distance)),
					radius, QPen(QColor(0, 255, 90), 2), QColor(35, 255, 125));
				else
					PaintCirle(painter, QPointF(y_origin.x() + column*(radius + distance),
					y_origin.y() + row*(radius + distance)),
					radius, QPen(QColor(255, 90, 90), 2), QColor(255, 45, 35));
			}
		}
	}
}

void TestDialog::PaintCirle(QPainter& painter, const QPointF& center_circle, int radius, const QPen &pen, const QBrush &brush)
{
	// 反走样
	painter.setRenderHint(QPainter::Antialiasing, true);
	// 设置画笔颜色、宽度,  边框
	painter.setPen(pen);
	// 设置画刷颜色
	painter.setBrush(brush);
	// 绘制圆
	painter.drawEllipse(center_circle, radius, radius);
}


void TestDialog::readyDataSlot(QByteArray str)
{
	if (!str.isEmpty())
	{
		std::string function_code = str.toStdString().substr(3, 2);
		if (function_code == "01")
			m_Y_States = Parse_Delta_Ascii(str.toStdString());

		if (function_code == "02")
		{
			m_X_States = Parse_Delta_Ascii(str.toStdString());
		}
			
		if (function_code == "03")
		{
			m_D_Register = Parse_Delta_Ascii_03(str.toStdString());	
			if (!m_D_Register.empty())
			{
				ui->lineEdit_distance->setText(QString("%1").arg(m_D_Register[0]));
				ui->lineEdit_time->setText(QString("%1").arg(m_D_Register[4]));
			}	
		}
			
	}
	else
	{
		//std::vector<bool> temp(40, false);
		m_Y_States.clear();
		m_X_States.clear();
		m_D_Register.clear();
	}
}