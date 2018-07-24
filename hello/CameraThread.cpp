#include "CameraThread.h"
#include <QMessageBox>
#include <QTime>
#include "Func.h"

#include <pylon/PylonIncludes.h>


//QMutex Camera_Thread::m_mutex;
//QMutex Camera_Thread::m_mutex_WriteData;
//QWaitCondition Camera_Thread::m_waitWriteData;
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
	QTime time;
	HTuple mean_gray;
	HObject Rectangle;
	while (!m_bIsStop)
	{
		try{
			//time.start();
			qDebug() << m_CameraId << " ready cap.... ";
			Image = m_pGrabber->GrabImage();
			//Image = m_pGrabber->GrabImageAsync(-1);

			qDebug() << endl << "Grab Image Info :" << m_CameraId;
			qDebug() << "=========================";
			qDebug() << "SizeX: " << (int)Image.Width();
			qDebug() << "SizeY: " << (int)Image.Height();
			qDebug() << "=========================" << endl;

			//if (isCorrectImage(Image,3.0))
			//{
				signal_image(&Image);
				//DispColor(Image, m_WindowHandle);
				QueueSaveImage(Image, m_MaxNum);
				//qDebug() << m_CameraId<<" all time: " << time.elapsed() / 1000.0;
			//}
			
		}
		catch (HException& e)
		{
			QString error = e.ErrorMessage().Text();
			if (!m_bIsStop)
			{
				qDebug() << m_CameraId << ": " << error ;
				continue;

			}		
			else
				break;
		}	
	}
	m_pGrabber->Clear();
	
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
			m_pGrabber->SetFramegrabberParam("exposure", -5);
			m_pGrabber->SetFramegrabberParam("grab_timeout", 5000);
			m_pGrabber->GrabImageStart(-1);
			
			break;

		case Camera_Thread::GigEVision:
			qDebug() << "ready Open Cam....";
			m_pGrabber->OpenFramegrabber("GigEVision", 1, 1, 0, 0, 0, 0, "default", 8, "gray", -1, "false", "default", \
				m_CameraId.toStdString().c_str(), 0, -1);
			qDebug() << "Open Cam OK";
		
			if (m_CameraId.contains("Basler"))
			{
				m_pGrabber->SetFramegrabberParam("PixelFormat", "Mono8");
				m_pGrabber->SetFramegrabberParam("Height", 10000);
				m_pGrabber->SetFramegrabberParam("TriggerSelector", "FrameStart");
				m_pGrabber->SetFramegrabberParam("TriggerMode", "Off");
				m_pGrabber->SetFramegrabberParam("TriggerSource", "Line1");
				m_pGrabber->SetFramegrabberParam("ExposureTimeRaw", 700);
				m_pGrabber->SetFramegrabberParam("AcquisitionLineRateAbs", 10000);
				m_pGrabber->SetFramegrabberParam("grab_timeout", 5000);
			}
			else if (m_CameraId.contains("DALSA"))
			{
				/*m_pGrabber->SetFramegrabberParam("AcquisitionLineRate", 10000.0);
				m_pGrabber->SetFramegrabberParam("ExposureTime", 80.0);
				m_pGrabber->SetFramegrabberParam("TriggerSelector", "FrameStart");
				m_pGrabber->SetFramegrabberParam("TriggerMode", "On");
				m_pGrabber->SetFramegrabberParam("TriggerSource", "Line1");
				m_pGrabber->SetFramegrabberParam("TriggerActivation", "RisingEdge");
				m_pGrabber->SetFramegrabberParam("LineSelector", "Line1");
				m_pGrabber->SetFramegrabberParam("LineFormat", "SingleEnded");
				m_pGrabber->SetFramegrabberParam("lineDetectionLevel", "Threshold_for_5V");
				m_pGrabber->SetFramegrabberParam("Height", 10000);
				m_pGrabber->SetFramegrabberParam("grab_timeout", 5000);*/

				m_pGrabber->SetFramegrabberParam("AcquisitionLineRate", 10000.0);
				m_pGrabber->SetFramegrabberParam("ExposureTime", 50.0);
				m_pGrabber->SetFramegrabberParam("TriggerSelector", "FrameStart");
				m_pGrabber->SetFramegrabberParam("TriggerMode", "Off");
				/*m_pGrabber->SetFramegrabberParam("TriggerSource", "Line1");
				m_pGrabber->SetFramegrabberParam("TriggerActivation", "RisingEdge");
				m_pGrabber->SetFramegrabberParam("LineSelector", "Line1");
				m_pGrabber->SetFramegrabberParam("LineFormat", "SingleEnded");
				m_pGrabber->SetFramegrabberParam("lineDetectionLevel", "Threshold_for_5V");*/
				m_pGrabber->SetFramegrabberParam("Height", 10000);
				m_pGrabber->SetFramegrabberParam("grab_timeout", -1);
			}

			//m_pGrabber->GrabImageStart(-1);

			break;

		default:
			break;
		}

		return true;
	}
	catch (HException& e)
	{
		//emit signal_error(e.ErrorMessage().Text());

		QString eror = e.ErrorMessage().Text();
		emit signal_error(G2U("不能获取相机，请检测相机ID是否正确"));
		return false;
	}
	catch (GenICam::AccessException& e)
	{
		QString error = e.GetDescription();
	}

}


void Camera_Thread::stop()
{
	m_bIsStop = true;
}

void Camera_Thread::setSaveDatePath(const QString& path)
{
	m_SaveDatePath = path;
}

void Camera_Thread::setSaveImageDirName(const QString& ImageDirName)
{
	m_SaveImageDirName = ImageDirName;
}

void Camera_Thread::QueueSaveImage(const HObject& Image,int maxnum)
{
	//QTime time;
	
	//if (CreateImagetDir())
	//{
	//	QVariant value;
	//	ReadConfigure("config.ini", "Config", "ImagePath1", value);
	//	setSaveDatePath(value.toString());
	//}
	

	if (m_image_index <= maxnum)
	{
		
		//QString saveImagePath = QString(m_SaveDatePath + "/" + m_SaveImageDirName + "/%1").arg(m_image_index, 4, 10, QChar('0'));
		QString saveImagePath = QString("images/" + m_SaveImageDirName + "/%1").arg(m_image_index, 4, 10, QChar('0'));
		//time.start();
		WriteImage(Image, "bmp", 0, saveImagePath.toStdString().c_str());
	//	qDebug() << "save time: " << time.elapsed() / 1000.0;

		m_image_index++;
	}
	else
	{
		m_image_index = 1;
		//QString saveImagePath = QString(m_SaveDatePath + "/" + m_SaveImageDirName + "/%1").arg(m_image_index, 4, 10, QChar('0'));
		QString saveImagePath = QString("images/" + m_SaveImageDirName + "/%1").arg(m_image_index, 4, 10, QChar('0'));
		WriteImage(Image, "bmp", 0, saveImagePath.toStdString().c_str());

		//QString saveAalPath = QString(m_SaveDatePath + "/aal/%1.aal").arg(m_image_index, 4, 10, QChar('0'));
		//WriteConfigure(saveAalPath, "Info", m_ConfigureName, saveImagePath + ".bmp");

		m_image_index++;
	}
}
