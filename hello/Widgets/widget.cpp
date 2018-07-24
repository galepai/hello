#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
	ui->verticalLayout->addStretch(1);
}

Widget::~Widget()
{
    delete ui;
}
