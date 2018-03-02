#include "configuredlg.h"
#include "ui_configuredlg.h"
#include <QtSerialPort\QSerialPortInfo>
#include <QMessageBox>
#include <qfiledialog.h>
#include"Func.h"
#include <QDebug>  
#include <QTime>  
#include <time.h>

ConfigureDlg::ConfigureDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigureDlg)
{
    ui->setupUi(this);

	QList<QSerialPortInfo>  infos = QSerialPortInfo::availablePorts();
	if (infos.isEmpty())
	{
		ui->PortComboBox->addItem("");
		return;
	}
	foreach(QSerialPortInfo info, infos) 
	{
		ui->PortComboBox->addItem(info.portName());
	}

	ui->BaudComboBox->addItem("4800");
	ui->BaudComboBox->addItem("9600");
	ui->BaudComboBox->addItem("19200");
	ui->BaudComboBox->setCurrentIndex(1);

	ui->DataBitsComboBox->addItem("6");
	ui->DataBitsComboBox->addItem("7");
	ui->DataBitsComboBox->addItem("8");
	ui->DataBitsComboBox->setCurrentIndex(2);

	QVariant Value;
	ReadConfigure("config.ini", "Raw", "Raw_Path", Value);
	ui->lineEdit_Raw_Path->setText(Value.toString());

	connect(ui->SavePortButton, SIGNAL(clicked()), this, SLOT(SavePortToIni()));
	connect(ui->SelectRawButton, SIGNAL(clicked()), this, SLOT(SelectRawPath()));
	connect(ui->SaveRawButton, SIGNAL(clicked()), this, SLOT(SaveRawPath()));

	HIDDLE_DIALOG_BUTTON

}

ConfigureDlg::~ConfigureDlg()
{
    delete ui;
}

void ConfigureDlg::SavePortToIni()
{


	double time_Start = (double)clock();

	if (ui->PortComboBox->currentText().length()
		&& ui->BaudComboBox->currentText().length()
		&& ui->DataBitsComboBox->currentText().length())
	{
		WriteConfigure("config.ini", "Port", "Port", ui->PortComboBox->currentText());
		WriteConfigure("config.ini", "Port", "Baud", ui->BaudComboBox->currentText());
		WriteConfigure("config.ini", "Port", "DataBits", ui->DataBitsComboBox->currentText());
		double time_End = (double)clock();

		qDebug() << (time_End - time_Start) / 1000.0 << "s";
		QMessageBox::StandardButton reply;
		reply = QMessageBox::information(this, G2U("信息"), G2U("串口信息成功写入配置文件"));
	}
	else
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::warning(this, G2U("信息"), G2U("串口信息写入失败，请检测设置是否正确！"));
	}
	

}

void ConfigureDlg::SelectRawPath()
{
	QString file_path = QFileDialog::getExistingDirectory(this, G2U("请选择原始数据保存路径..."), "./");
	if (file_path.isEmpty())
	{
		return;
	}
	else
	{
		ui->lineEdit_Raw_Path->setText(file_path);
	}
}

void ConfigureDlg::SaveRawPath()
{
	if (ui->lineEdit_Raw_Path->text().isEmpty())
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::warning(this, G2U("警告"), G2U("请选择路径……"));
	}
	else
	{
		QTime time;
		time.start();
		WriteConfigure("config.ini", "Raw", "Raw_Path", ui->lineEdit_Raw_Path->text());
		qDebug() << time.elapsed() / 1000.0 << "s";
		QMessageBox::StandardButton reply;
		reply = QMessageBox::information(this, G2U("信息"), G2U("成功写入配置文件。"));
	}
	
}