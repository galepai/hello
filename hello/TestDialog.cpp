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
	connect(ui->btn_shuiping_zhuashou_left, SIGNAL(clicked()), this, SLOT(OnShuiPingLeft()));
	connect(ui->btn_shuiping_zhuashou_right, SIGNAL(clicked()), this, SLOT(OnShuiPingRight()));
	connect(ui->btn_hand_open, SIGNAL(clicked()), this, SLOT(OnHandOpen()));
	connect(ui->btn_hand_close, SIGNAL(clicked()), this, SLOT(OnHandClose()));


	QList<QPushButton *> pPushButtons = findChildren<QPushButton *>();
	for (int i = 0; i < pPushButtons.count(); i++)
	{
		pPushButtons.at(i)->setProperty("status",i>5);
	}

	m_pTimer = new QTimer(this);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(update()));
	m_pTimer->start(200);

	QVariant value;
	ReadConfigure("config.ini", "Port", "Port", value);
	QString PortName = value.toString();
	ReadConfigure("config.ini", "Port", "Baud", value);
	int Baud = value.toInt();
	ReadConfigure("config.ini", "Port", "DataBits", value);
	int DataBits = value.toInt();

	//设置默认查询队列
	Delta_Thread::setQueryMode(Delta_Thread::QueryMode::DefalutQuene);
	Delta_Thread::AddDefaultQueueInfo("000204000028");	//读Y00-Y47

	if (!Delta_Thread::GetSerialPort())
	{
		Delta_Thread* thread = new Delta_Thread;
		thread->InitSerialPortInfo(PortName.toStdString().c_str(), Baud, QSerialPort::Parity::EvenParity, QSerialPort::DataBits(DataBits));
		connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
		connect(thread, SIGNAL(emitdata(QByteArray)), this, SLOT(readyDataSlot(QByteArray)));
		thread->start();
	}
		
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
	//Delta_Thread::AddOneQueueInfo("000204000028");	//读Y00-Y47
	Delta_Thread::StopRun(true);
	close();
}

void TestDialog::OnShuiPingLeft()
{
	Delta_Thread::AddOneQueueInfo(SHUIPING_ZHUASHOU_LEFT_ON);
	Delta_Thread::AddOneQueueInfo(SHUIPING_ZHUASHOU_LEFT_OFF);
}

void TestDialog::OnShuiPingRight()
{
	Delta_Thread::AddOneQueueInfo(SHUIPING_ZHUASHOU_RIGHT_ON);	
	Delta_Thread::AddOneQueueInfo(SHUIPING_ZHUASHOU_RIGHT_OFF);	
}

void TestDialog::OnHandOpen()
{
	Delta_Thread::AddOneQueueInfo(SHUIPING_HAND_OPEN_ON);
	Delta_Thread::AddOneQueueInfo(SHUIPING_HAND_OPEN_OFF);
}

void TestDialog::OnHandClose()
{
	Delta_Thread::AddOneQueueInfo(SHUIPING_HAND_CLOSE_ON);
	Delta_Thread::AddOneQueueInfo(SHUIPING_HAND_CLOSE_OFF);
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

	QList<QPushButton *> pPushButtons = findChildren<QPushButton *>();
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