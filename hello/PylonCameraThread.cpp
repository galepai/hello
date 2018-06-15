#include "PylonCameraThread.h"
#include <QMessageBox>
#include <QTime>
#include "Func.h"
#include "ConstParam.h"


QStringList PylonCamera_Thread::m_CameraIdlist;

PylonCamera_Thread::PylonCamera_Thread(ConnectionType connection_type,QString CameraId, QObject *parent)
	: m_connectionType(connection_type),m_CameraId(CameraId),QThread(parent)
{
	m_image_index = 1;
	m_CameraIdlist.append(CameraId);
}

void PylonCamera_Thread::run()
{

	if (!OpenCamera())
		return;

	m_bIsStop = false;
	HImage Image;
	QTime time;
	while (!m_bIsStop)
	{
		try{
			time.start();
			qDebug() << endl << m_CameraId << " ready cap.... ";

			//m_camera.StartGrabbing();
			CGrabResultPtr ptrGrabResult;

			//if (m_camera.WaitForFrameTriggerReady(5000))
			//{
			//	m_camera.RetrieveResult(5000, ptrGrabResult);

			//	if (ptrGrabResult->GrabSucceeded())
			//	{
			//		// Access the image data.
			//		qDebug() << endl << "Grab Image Info";
			//		qDebug() << "=========================";
			//		qDebug() << "SizeX: " << ptrGrabResult->GetWidth();
			//		qDebug() << "SizeY: " << ptrGrabResult->GetHeight();
			//		const uint8_t* pImageBuffer = (uint8_t *)ptrGrabResult->GetBuffer();
			//		qDebug() << "Gray value of first pixel: " << (uint32_t)pImageBuffer[0];
			//		qDebug() << "=========================" << endl;
			//		GenImage1(&Image, "byte", (int)ptrGrabResult->GetWidth(), (int)ptrGrabResult->GetHeight(), (Hlong)pImageBuffer);

			//	}
			//	else
			//	{
			//		qDebug() << "Error: " << ptrGrabResult->GetErrorCode() << " " << ptrGrabResult->GetErrorDescription() << endl;
			//	}
			//}
			
			//if (m_camera.IsGrabbing())
			if (m_camera.GrabOne(5000, ptrGrabResult))
			{
				if (ptrGrabResult->GrabSucceeded())
				{
					// Access the image data.
					qDebug() << endl << "Grab Image Info :" << m_CameraId;
					qDebug() << "=========================" ;
					qDebug() << "SizeX: " << ptrGrabResult->GetWidth();
					qDebug() << "SizeY: " << ptrGrabResult->GetHeight();
					const uint8_t* pImageBuffer = (uint8_t *)ptrGrabResult->GetBuffer();
					qDebug() << m_CameraId << "	:	" << pImageBuffer;
					qDebug() << "Gray value of first pixel: " << (uint32_t)pImageBuffer[0];
					qDebug() << "========================="  << endl;
					GenImage1(&Image, "byte", (int)ptrGrabResult->GetWidth(), (int)ptrGrabResult->GetHeight(), (Hlong)pImageBuffer);

				}
				else
				{
					qDebug() << "Error: " << ptrGrabResult->GetErrorCode() << " " << ptrGrabResult->GetErrorDescription() << endl;
				}
			}
		

			signal_image(&Image);
			QueueSaveImage(Image, m_MaxNum);
			qDebug() << m_CameraId<<" all time: " << time.elapsed() / 1000.0;
		}
		catch (HException& e)
		{
			QString error = e.ErrorMessage().Text();
			if (!m_bIsStop)
			{
				qDebug() << m_CameraId << error ;
				continue;

			}		
			else
				break;
		}
		catch (const GenericException &e)
		{
			// Error handling.
			qDebug() << "An exception occurred in run:" << endl
				<< e.GetDescription() << endl;
		}
	}
	m_camera.Close();
	m_camera.DetachDevice();

	// Releases all pylon resources. 
	PylonTerminate();
	
	
}

PylonCamera_Thread::~PylonCamera_Thread()
{
	//m_CameraIdlist.removeAll(m_CameraId);
}

bool PylonCamera_Thread::IsExistCameraId(QString cameraId)
{
	return m_CameraIdlist.contains(cameraId);
}


QString PylonCamera_Thread::CameraId() const
{
	return m_CameraId;
}

bool PylonCamera_Thread::OpenCamera()
{

	/*uint c_countOfImagesToGrab = 5;
	uint c_maxCamerasToUse = 2;*/
	try{
		switch (m_connectionType)
		{
		case PylonCamera_Thread::DirectShow:
			
		
			break;

		case PylonCamera_Thread::GigEVision:
	
			// Before using any pylon methods, the pylon runtime must be initialized. 
			PylonInitialize();

			try
			{
				m_camera.Attach(CTlFactory::GetInstance().CreateDevice(m_CameraId.toStdString().c_str()));
			
				using namespace GenApi;
				INodeMap& nodemap = m_camera.GetNodeMap();
				
				m_camera.Open();
				
				 //Get camera device information.
				qDebug() << endl << "Camera Device Information" << endl
					<< "=========================";
				qDebug() << "Vendor					: "
					<< CStringPtr(nodemap.GetNode("DeviceVendorName"))->GetValue();
				qDebug() << "Model					: "
					<< CStringPtr(nodemap.GetNode("DeviceModelName"))->GetValue();
				qDebug() << "Firmware version		: "
					<< CStringPtr(nodemap.GetNode("DeviceFirmwareVersion"))->GetValue();
				qDebug() << "Using Device FullInfo	: " 
					<< m_camera.GetDeviceInfo().GetFullName();


				// Camera settings.
				qDebug() << endl << "Camera Device Settings" << endl
					<< "======================" << endl;

				CIntegerPtr Width(nodemap.GetNode("Width"));
				CIntegerPtr Height(nodemap.GetNode("Height"));
				CIntegerPtr ExposureTimeRaw(nodemap.GetNode("ExposureTimeRaw"));
				CFloatPtr	AcquisitionLineRateAbs(nodemap.GetNode("AcquisitionLineRateAbs"));
				CEnumerationPtr TriggerSelector(nodemap.GetNode("TriggerSelector"));
				CEnumerationPtr TriggerMode(nodemap.GetNode("TriggerMode"));
				CEnumerationPtr	TriggerSource(nodemap.GetNode("TriggerSource"));
				CEnumerationPtr TriggerActivation(nodemap.GetNode("TriggerActivation"));
			
				Width->SetValue(2048);
				Height->SetValue(10000);
				ExposureTimeRaw->SetValue(760);
				AcquisitionLineRateAbs->SetValue(10000.0);
				TriggerSelector->FromString("FrameStart");
				TriggerMode->FromString("Off");
				TriggerSource->FromString("Line1");
				TriggerActivation->FromString("RisingEdge");
			

				qDebug() << "Width					: " << Width->GetValue();
				qDebug() << "Height					: " << Height->GetValue();
				qDebug() << "ExposureTimeRaw			: " << ExposureTimeRaw->GetValue();
				qDebug() << "AcquisitionLineRateAbs	: " << AcquisitionLineRateAbs->GetValue();
				qDebug() << "TriggerSelector			: " << TriggerSelector->ToString();
				qDebug() << "TriggerMode				: " << TriggerMode->ToString();
				qDebug() << "TriggerSource			: " << TriggerSource->ToString();
				qDebug() << "TriggerActivation		: " << TriggerActivation->ToString();

				qDebug() << "======================" << endl;
				// The parameter MaxNumBuffer can be used to control the count of buffers
				// allocated for grabbing. The default value of this parameter is 10.
				m_camera.MaxNumBuffer = 3;

			}
			catch (const GenericException &e)
			{
					// Error handling.
					qDebug() << "An exception occurred." << endl
						<< e.GetDescription() << endl;
					//m_camera.Close();
					//m_camera.DetachDevice();

					return false;
			}

			break;

		default:

			break;
		}

		return true;
	}
	//catch (HException& e)
	//{
	//	//emit signal_error(e.ErrorMessage().Text());

	//	QString eror = e.ErrorMessage().Text();
	//	emit signal_error(G2U("不能获取相机，请检测相机ID是否正确"));
	//	return false;
	//}
	catch (GenICam::AccessException& e)
	{
		QString  error = e.GetDescription();
		emit signal_error(error);
		return false;
	}

}


void PylonCamera_Thread::stop()
{
	m_bIsStop = true;
}

void PylonCamera_Thread::setSaveDatePath(const QString& path)
{
	m_SaveDatePath = path;
}

void PylonCamera_Thread::setSaveImageDirName(const QString& ImageDirName)
{
	m_SaveImageDirName = ImageDirName;
}

void PylonCamera_Thread::QueueSaveImage(const HObject& Image,int maxnum)
{
	//QTime time;
	
	if (CreateImagetDir())
	{
		QVariant value;
		ReadConfigure("config.ini", "Config", "ImagePath1", value);
		setSaveDatePath(value.toString());
	}
	

	if (m_image_index <= maxnum)
	{
		
		QString saveImagePath = QString(m_SaveDatePath + "/" + m_SaveImageDirName + "/%1").arg(m_image_index, 4, 10, QChar('0'));
		//time.start();
		WriteImage(Image, "bmp", 0, saveImagePath.toStdString().c_str());
	//	qDebug() << "save time: " << time.elapsed() / 1000.0;

		m_image_index++;
	}
	else
	{
		m_image_index = 1;
		QString saveImagePath = QString(m_SaveDatePath + "/" + m_SaveImageDirName + "/%1").arg(m_image_index, 4, 10, QChar('0'));
		WriteImage(Image, "bmp", 0, saveImagePath.toStdString().c_str());

		//QString saveAalPath = QString(m_SaveDatePath + "/aal/%1.aal").arg(m_image_index, 4, 10, QChar('0'));
		//WriteConfigure(saveAalPath, "Info", m_ConfigureName, saveImagePath + ".bmp");

		m_image_index++;
	}
}
