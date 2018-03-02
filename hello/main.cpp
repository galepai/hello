#include "hello.h"
#include <QtWidgets/QApplication>
#include "Func.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	hello w;
	//w.setWindowTitle(G2U("ÄÚ´æ"));
	w.showMaximized();

	return a.exec();
}

