#ifndef PIC_THREAD_LEFT_H
#define PIC_THREAD_LEFT_H

#include <QThread>
#include <QDebug>
#include "ConstParam.h"
#include <HalconCpp.h>
using namespace HalconCpp;

class PicThreadLeft : public QThread
{
	Q_OBJECT

public:
	explicit PicThreadLeft(QObject *parent = 0)
		: QThread(parent)
	{
		//qDebug() << "Worker Thread : " << QThread::currentThreadId();
	}


	HImage m_Image;
	HTuple m_WindowHandle;

protected:
	virtual void run() Q_DECL_OVERRIDE;

signals:
	void resultReady(int bIsBad);



};

#endif