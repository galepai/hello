/**
*	功能：台达DVP_PLC线程。
*	作者：chenhui
*/

#ifndef CAMERA_THREAD_H
#define CAMERA_THREAD_H

#include <QThread>
#include <QDebug>
#include <QWaitCondition>
#include <atomic>
#include <QMutex>
#include "HalconCpp.h"

using namespace HalconCpp;
class Camera_Thread : public QThread
{
	Q_OBJECT
	
public:
	enum ConnectionType
	{
		DirectShow,	//USB连接
		GigEVision,	//网线连接
	};

	explicit Camera_Thread(ConnectionType connection_type, QString CameraId, QObject *parent = 0);
	~Camera_Thread();
	
	QString CameraId() const;
	void setSaveImagePath(const QString& path);
	void stop();

protected:
	virtual void run() Q_DECL_OVERRIDE;

	bool OpenCamera();
	void setConnection_Type(ConnectionType connection_type);
	static QMutex m_mutex;
	static QMutex m_mutex_WriteData;
	static QWaitCondition m_waitWriteData;
	void QueueSaveImage(const HObject& Image, int maxnum);
	

signals:
	void signal_image(void* pimage);
	void signal_error(QString error);

private:
	QString m_CameraId;
	ConnectionType m_connectionType;
	HFramegrabber* pGrabber;
	bool m_bIsStop;
	QString m_SaveImagePath;
	int m_image_index;

};

#endif