#include "TestDialog.h"
#include "ui_TestDialog.h"
#include "DeltaThread.h"
#include "Func.h"
#include <QPushButton>
#include <QLineEdit>


TestDialog::TestDialog(QWidget *parent) :
    QDialog(parent),
	ui(new Ui::TestDialog)
{
    ui->setupUi(this);
	connect(ui->btn_return, SIGNAL(clicked()), this, SLOT(CloseWindow()));
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
	close();
}

TestDialog::~TestDialog()
{
    delete ui;
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
	/*QLineEdit{
		border: 1px solid rgb(41, 57, 85);   # 边框1px宽，颜色为深紫色
		border-radius: 3px;					# 给定3px边框圆角
		background : white;			# 背景色定为白色吧
		selection-background-color: green;	# 这个属性设定文本选中时的文本背景色
		font-size: 14px;  # 文本的大小
	}*/
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