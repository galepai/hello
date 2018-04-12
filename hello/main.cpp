#include "hello.h"
#include <QtWidgets/QApplication>
#include <QFile>
#include <QDir>
#include "Func.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QFile qss("Resources/qss/css.h");
	if (qss.open(QFile::ReadOnly))
	{
		qApp->setStyleSheet(qss.readAll());
		qss.close();
	}
	
	hello w;
	//w.setWindowTitle(G2U("ÄÚ´æ"));
	w.showMaximized();

	return a.exec();
}

