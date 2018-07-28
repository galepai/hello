#ifndef HELLO_H
#define HELLO_H

#include <QtWidgets/QMainWindow>
#include "ui_hello.h"
#include "HalconCpp.h"
#include "HalconCpp.h"
#include "Func.h"
#include "TestDialog.h"
#include "CameraThread.h"
#include "PylonCameraThread.h"

using namespace HalconCpp;

class hello : public QMainWindow
{
	Q_OBJECT

public:
	hello(QWidget *parent = 0);
	~hello();

	//bool getIsBadStatu(){ return m_AllResult; };
	bool OpenSerial();

	//*********ͼ�����߳�**************/
	void HandleImageThread(HImage& ima, LocationView view);

public slots:

	/***********�˵�������************/
	void OnLRC();
	void OnAbout();

	/***********����������************/
	void OnOneHandle();
	void OnOpen();
	void OnLineRun();
	void OnConfigure();
	void OnShutDown();	//�ر�ϵͳ
	void OnTest();

	/***********�Ҷ˵�������************/
	void DebugDialog();
	void ChangeMode(int mode);
	void OnStart();
	void OnStop();
	void OnDetectEnd();

	//
	void handleResults(int is_bad);
	void readyDataSlot(QByteArray str);
	void receiveLeftImage(void* image);
	void receiveMiddleImage(void* image);
	void receiveRightImage(void* image);
	void receiveError(QString error);
	void receiveCorrectImage(int value);

private:
	void OnClearCameraThread();	//��������߳�
	void FullScreenShow();	//ȫ����ʾ
	void DispPic(HImage& path, LocationView location);	//��ʾͼƬ��ָ��λ����ͼ
	const HalconCpp::HTuple GetViewWindowHandle(LocationView location);	//����ָ����ͼ�Ĵ���Handle��Halcon��ʾͼƬ�ã�
	void SetOpenWindowHandle(HImage& Image, HTuple* pWindowHandle, LocationView location);	//ָ����ʾ���ڵľ��
	void SetPicViewScroll(int width, int height, LocationView location); //ָ����ͼ�Ĺ���������


private:
	Ui::helloClass ui;
	HalconCpp::HTuple m_LeftWindowHandle, m_MiddleWindowHandle, m_RightWindowHandle;	//���С�����ͼ�Ĵ���Handle��Halcon��ʾͼƬ�ã�
	QString m_Title;
	HImage m_Image;
	int m_AllResult;

signals:
	void ReadyLoop();

public:
	Camera_Thread* m_camera_thread1;
	Camera_Thread* m_camera_thread2;
	
	PylonCamera_Thread* m_Pylon_camera_thread1;
	PylonCamera_Thread* m_Pylon_camera_thread2;
};

#endif // HELLO_H
