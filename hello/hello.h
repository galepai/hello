#ifndef HELLO_H
#define HELLO_H

#include <QtWidgets/QMainWindow>
#include "ui_hello.h"
#include "HalconCpp.h"
#include "HandlePicThread.h"
#include "HalconCpp.h"


using namespace HalconCpp;

class hello : public QMainWindow
{
	Q_OBJECT

public:
	hello(QWidget *parent = 0);
	~hello();

	bool getIsBadStatu(){ return m_bIsBad; };

	//QObject* pobject;

public slots:
	void click();
	void Open();
	void About();
	void OnLineRun();
	void OnConfigure2();
	void OnShutDown();
	void handleResults(bool is_bad);
	void readyDataSlot(QByteArray str);
	

private:
	void DispPic(HImage& path);

private:
	Ui::helloClass ui;
	HalconCpp::HTuple m_WindowHandle;
	QString m_Title;
	HandlePicThread *m_pHandlePicThread;
	HImage m_Image;
	bool m_bIsBad;
};

#endif // HELLO_H
