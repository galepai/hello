#ifndef PIC_THREAD_MIDDLE_H
#define PIC_THREAD_MIDDLE_H

#include <QThread>
#include <QDebug>
#include "ConstParam.h"
#include <HalconCpp.h>
using namespace HalconCpp;

class PicThreadMiddle : public QThread
{
	Q_OBJECT

public:
	explicit PicThreadMiddle(QObject *parent = 0)
		: QThread(parent)
	{
		//qDebug() << "Worker Thread : " << QThread::currentThreadId();
	}

	void OnHandle(HTuple WindowHandle);

	static int num;
	HImage m_Image;
	HTuple m_WindowHandle;

protected:
	virtual void run() Q_DECL_OVERRIDE;

signals:
	void resultReady(int bIsBad);



};

#endif