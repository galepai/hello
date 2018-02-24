#ifndef HANDLE_PIC_THREAD_H
#define HANDLE_PIC_THREAD_H

#include <QThread>
#include <QDebug>
#include <HalconCpp.h>
using namespace HalconCpp;

class HandlePicThread : public QThread
{
	Q_OBJECT

public:
	explicit HandlePicThread(QObject *parent = 0)
		: QThread(parent)
	{
		qDebug() << "Worker Thread : " << QThread::currentThreadId();
	}


	HImage m_Image;
	HTuple m_WindowHandle;

protected:
	virtual void run() Q_DECL_OVERRIDE;

signals:
	void resultReady(bool bIsBad);



};

#endif