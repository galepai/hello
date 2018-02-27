#include "configuredlg.h"
#include "ui_configuredlg.h"
#include <QtSerialPort\QSerialPortInfo>
#include <QMessageBox>
#include"Func.h"

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

	connect(ui->SavePortButton, SIGNAL(clicked()), this, SLOT(SavePortToIni()));
	
}

ConfigureDlg::~ConfigureDlg()
{
    delete ui;
}

void ConfigureDlg::SavePortToIni()
{

	if (ui->PortComboBox->currentText().length()
		&& ui->BaudComboBox->currentText().length()
		&& ui->DataBitsComboBox->currentText().length())
	{
		WriteConfigure("config.ini", "Port", "Port", ui->PortComboBox->currentText());
		WriteConfigure("config.ini", "Port", "Baud", ui->BaudComboBox->currentText());
		WriteConfigure("config.ini", "Port", "DataBits", ui->DataBitsComboBox->currentText());

		QMessageBox::StandardButton reply;
		reply = QMessageBox::information(this, G2U("��Ϣ"), G2U("������Ϣ�ɹ�д�������ļ�"));
	}
	else
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::warning(this, G2U("��Ϣ"), G2U("������Ϣд��ʧ�ܣ����������Ƿ���ȷ��"));
	}

}