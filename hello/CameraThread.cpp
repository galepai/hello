#include "CameraThread.h"
#include <QMessageBox>
#include <QTime>
#include "Func.h"


QMutex Camera_Thread::m_mutex;
QMutex Camera_Thread::m_mutex_WriteData;
QWaitCondition Camera_Thread::m_waitWriteData;
QStringList Camera_Thread::m_CameraIdlist;

Camera_Thread::Camera_Thread(ConnectionType connection_type,QString CameraId, QObject *parent)
	: m_connectionType(connection_type),m_CameraId(CameraId),QThread(parent)
{
	m_image_index = 1;
	m_CameraIdlist.append(CameraId);
	m_pGrabber = nullptr;
}

void Camera_Thread::run()
{

	if (!OpenCamera())
		return;

	m_bIsStop = false;
	HImage Image;
	while (!m_bIsStop)
	{
		//Image = pGrabber->GrabImageAsync(-1);
		Image = m_pGrabber->GrabImage();
		//GrabImageAsync(&Image, hv_AcqHandle, -1);
		signal_image(&Image);
		QueueSaveImage(Image, m_MaxNum);
	}
	m_pGrabber->Clear();
	//CloseFramegrabber(hv_AcqHandle);
	
}

Camera_Thread::~Camera_Thread()
{
	m_CameraIdlist.removeAll(m_CameraId);
	delete m_pGrabber;
	m_pGrabber = nullptr;
}

bool Camera_Thread::IsExistCameraId(QString cameraId)
{
	return m_CameraIdlist.contains(cameraId);
}


QString Camera_Thread::CameraId() const
{
	return m_CameraId;
}

bool Camera_Thread::OpenCamera()
{
	m_pGrabber = new HalconCpp::HFramegrabber;

	try{
		switch (m_connectionType)
		{
		case Camera_Thread::DirectShow:
			m_pGrabber->OpenFramegrabber("DirectShow", 1, 1, 0, 0, 0, 0, "default", 8, "rgb", -1, "false", "default", \
				m_CameraId.toStdString().c_str(), 0, -1);
			m_pGrabber->SetFramegrabberParam("exposure", -1);
			//pGrabber->GrabImageStart(-1);
			break;

		case Camera_Thread::GigEVision:
			m_pGrabber->OpenFramegrabber("GigEVision", 1, 1, 0, 0, 0, 0, "default", 8, "gray", -1, "false", "default", \
				m_CameraId.toStdString().c_str(), 0, -1);
			//m_pGrabber->OpenFramegrabber("GigEVision", 1, 1, 0, 0, 0, 0, "default", 8, "gray", -1, "false", "default", \
				//"003053255252_Basler_raL204848gm", 0, -1);
			if (m_CameraId.contains("Basler"))
			{
				m_pGrabber->SetFramegrabberParam("PixelFormat", "Mono8");
				m_pGrabber->SetFramegrabberParam("Height", 10000);
				m_pGrabber->SetFramegrabberParam("TriggerSelector", "FrameStart");
				m_pGrabber->SetFramegrabberParam("TriggerMode", "Off");
				m_pGrabber->SetFramegrabberParam("ExposureTimeRaw", 550);
				m_pGrabber->SetFramegrabberParam("AcquisitionLineRateAbs", 12285.0);
			}
			else if (m_CameraId.contains("DALSA"))
			{
				m_pGrabber->SetFramegrabberParam("AcquisitionLineRate", 10000.0);
				m_pGrabber->SetFramegrabberParam("ExposureTime", 50.0);
				m_pGrabber->SetFramegrabberParam("TriggerSelector", "FrameStart");
				m_pGrabber->SetFramegrabberParam("TriggerMode", "On");
				m_pGrabber->SetFramegrabberParam("TriggerSource", "Line1");
				m_pGrabber->SetFramegrabberParam("TriggerActivation", "RisingEdge");
				m_pGrabber->SetFramegrabberParam("LineSelector", "Line1");
				m_pGrabber->SetFramegrabberParam("LineFormat", "SingleEnded");
				m_pGrabber->SetFramegrabberParam("lineDetectionLevel", "Threshold_for_5V");
				m_pGrabber->SetFramegrabberParam("Height", 10000);
				m_pGrabber->SetFramegrabberParam("grab_timeout", -1);
			}

			m_pGrabber->GrabImageStart(-1);
			break;

		default:
			break;
		}

		return true;
	}
	catch (HException& e)
	{
		//emit signal_error(e.ErrorMessage().Text());
		emit signal_error(G2U("不能获取相机，请检测相机ID是否正确"));
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
