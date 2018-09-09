#include "modelWidget.h"
#include "ui_modelWidget.h"
#include "Func.h"
#include <QMessageBox>
#include <qdebug.h>
#include <QFileDialog>

modelWidget::modelWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::modelWidget)
{
    ui->setupUi(this);
	ui->verticalLayout_main->addStretch(1);

	ReadIni();

	connect(ui->confirmButton, SIGNAL(clicked()), this, SLOT(SaveExposureToIni()));
	connect(ui->pushButton_loadGouModel, SIGNAL(clicked()), this, SLOT(loadGouModel()));
}

modelWidget::~modelWidget()
{
    delete ui;
}

void modelWidget::ReadIni()
{

	QVariant Value;
	ReadConfigure("config.ini", "Model", "GouModel", Value);
	ui->label_gouModel->setText(Value.toString());

	ReadConfigure("config.ini", "Camera_07_Clock", "Exposure", Value);
	ui->spinBox_07->setValue(Value.toInt());

	ReadConfigure("config.ini", "Camera_10_Clock", "Exposure", Value);
	ui->spinBox_10->setValue(Value.toInt());

	ReadConfigure("config.ini", "Camera_11_Clock", "Exposure", Value);
	ui->spinBox_11->setValue(Value.toInt());

}

void modelWidget::SaveExposureToIni()
{
		WriteConfigure("config.ini", "Model", "GouModel", ui->label_gouModel->text());
		WriteConfigure("config.ini", "Camera_07_Clock", "Exposure", QString("%1").arg(ui->spinBox_07->value()));
		WriteConfigure("config.ini", "Camera_10_Clock", "Exposure", QString("%1").arg(ui->spinBox_10->value()));
		WriteConfigure("config.ini", "Camera_11_Clock", "Exposure", QString("%1").arg(ui->spinBox_11->value()));

		QMessageBox::StandardButton reply;
		reply = QMessageBox::information(this, G2U("信息"), G2U("曝光参数成功写入配置文件"));
	
}

void modelWidget::loadGouModel()
{

	QString path = QFileDialog::getOpenFileName(this, tr("Open Model"), "model", tr("Model Files(*.gmc)"));
	ui->label_gouModel->setText(path);
}