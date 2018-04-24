#include "CameraThread.h"
#include <QMessageBox>
#include <QTime>
#include "Func.h"


QMutex Camera_Thread::m_mutex;
QMutex Camera_Thread::m_mutex_WriteData;
QWaitCondition Camera_Thread::m_waitWriteData;

Camera_Thread::Camera_Thread(ConnectionType connection_type,QString CameraId, QObject *parent)
	: m_connectionType(connection_type),m_CameraId(CameraId),QThread(parent)
{
	m_image_index = 1;

}

void Camera_Thread::run()
{

	if (!OpenCamera())
		return;

	m_bIsStop = false;
	HImage Image;
	while (!m_bIsStop)
	{
		Image = pGrabber->GrabImageAsync(-1);
		//GrabImageAsync(&Image, hv_AcqHandle, -1);
		signal_image(&Image);
		QueueSaveImage(Image, 50);
	}
	pGrabber->Clear();
	//CloseFramegrabber(hv_AcqHandle);
	
}

Camera_Thread::~Camera_Thread()
{
	delete pGrabber;
	pGrabber = nullptr;
}


QString Camera_Thread::CameraId() const
{
	return m_CameraId;
}

bool Camera_Thread::OpenCamera()
{
	pGrabber = new HalconCpp::HFramegrabber;

	try{
		switch (m_connectionType)
		{
		case Camera_Thread::DirectShow:
			pGrabber->OpenFramegrabber("DirectShow", 1, 1, 0, 0, 0, 0, "default", 8, "gray", -1, "false", "default", \
				m_CameraId.toStdString().c_str(), 0, -1);
			pGrabber->SetFramegrabberParam("exposure", -2);
			pGrabber->GrabImageStart(-1);
			break;

		case Camera_Thread::GigEVision:
			pGrabber->OpenFramegrabber("GigEVision", 1, 1, 0, 0, 0, 0, "default", 8, "gray", -1, "false", "default", \
				m_CameraId.toStdString().c_str(), 0, -1);
			pGrabber->SetFramegrabberParam("PixelFormat", "Mono8");
			pGrabber->SetFramegrabberParam("Height", 12288);
			pGrabber->SetFramegrabberParam("TriggerSelector", "FrameStart");
			pGrabber->SetFramegrabberParam("TriggerMode", "Off");
			//pGrabber->SetFramegrabberParam ( "ExposureTimeRaw", 550);
			//pGrabber->SetFramegrabberParam ( "AcquisitionLineRateAbs", 12285.0);
			break;

		default:
			break;
		}

		return true;
	}
	catch (HException& e)
	{
		emit signal_error(e.ErrorMessage().Text());
		return false;
	}
}


void Camera_Thread::stop()
{
	m_bIsStop = true;
}

void Camera_Thread::setSaveImagePath(const QString& path)
{
	m_SaveImagePath = path;
}

void Camera_Thread::QueueSaveImage(const HObject& Image,int maxnum)
{
	if (m_image_index <= maxnum)
	{
		QString savePath = QString(m_SaveImagePath + "/%1").arg(m_image_index,4,10,QChar('0'));
		WriteImage(Image, "bmp", 0, savePath.toStdString().c_str());
		m_image_index++;
	}
	else
	{
		m_image_index = 1;
		QString savePath = QString(m_SaveImagePath + "/%1").arg(m_image_index, 4, 10, QChar('0'));
		WriteImage(Image, "bmp", 0, savePath.toStdString().c_str());
		m_image_index++;
	}
}
