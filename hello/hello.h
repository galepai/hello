#ifndef HELLO_H
#define HELLO_H

#include <QtWidgets/QMainWindow>
#include "ui_hello.h"
#include "HalconCpp.h"
#include "HandlePicThread.h"
#include "HalconCpp.h"
#include "Func.h"

using namespace HalconCpp;

class hello : public QMainWindow
{
	Q_OBJECT

public:
	hello(QWidget *parent = 0);
	~hello();

	bool getIsBadStatu(){ return m_bIsBad; };

public slots:
	void click();
	void Open();
	void About();
	void OnLineRun();
	void OnConfigure2();
	void OnShutDown();	//�ر�ϵͳ
	void handleResults(bool is_bad);
	void readyDataSlot(QByteArray str);
	

private:
	void DispPic(HImage& path, LocationView location);	//��ʾͼƬ��ָ��λ����ͼ
	const HalconCpp::HTuple GetViewWindowHandle(LocationView location);	//����ָ����ͼ�Ĵ���Handle��Halcon��ʾͼƬ�ã�

private:
	Ui::helloClass ui;
	HalconCpp::HTuple m_LeftWindowHandle, m_MiddleWindowHandle, m_RightWindowHandle;	//���С�����ͼ�Ĵ���Handle��Halcon��ʾͼƬ�ã�
	QString m_Title;
	HandlePicThread *m_pHandlePicThread;
	HImage m_Image;
	bool m_bIsBad;

signals:
	void ReadyLoop();
};

#endif // HELLO_H
