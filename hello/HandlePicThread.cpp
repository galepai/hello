#include "HandlePicThread.h"


//void HandlePicThread::run()
//{
//	qDebug() << "Worker Run Thread : " << QThread::currentThreadId();
//	int nValue = 0;
//	while (nValue < 100)
//	{
//		// ����50����
//		msleep(50);
//		++nValue;
//		//׼������
//		emit resultReady(nValue);
//	}
//}

void HandlePicThread::run()
{
	//qDebug() << "Worker Run Thread : " << QThread::currentThreadId();

	if (m_Image.Key() != 0)
	{
		bool isbad = false;
		try
		{
			HObject Region;
			Threshold(m_Image, &Region, 5, 200);
			SetDraw(m_WindowHandle, "margin");
			SetColor(m_WindowHandle, "red");
			DispObj(Region, m_WindowHandle);

			/*if ()
			{*/
			isbad = true;
			//}
			emit resultReady(isbad);
		}
		catch (HException& e)
		{
			QString error = e.ErrorMessage().Text();
		}
	}
		
}