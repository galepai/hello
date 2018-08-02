#include "cameraWidget.h"
#include "ui_cameraWidget.h"

cameraWidget::cameraWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cameraWidget)
{
    ui->setupUi(this);
	ui->verticalLayout_main->addStretch(1);
}

cameraWidget::~cameraWidget()
{
    delete ui;
}
