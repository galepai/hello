#include "HandlePicThread.h"


//void HandlePicThread::run()
//{
//	qDebug() << "Worker Run Thread : " << QThread::currentThreadId();
//	int nValue = 0;
//	while (nValue < 100)
//	{
//		// 休眠50毫秒
//		msleep(50);
//		++nValue;
//		//准备更新
//		emit resultReady(nValue);
//	}
//}

void HandlePicThread::run()
{
	qDebug() << "Worker Run Thread : " << QThread::currentThreadId();

	if (m_Image.Key() != 0)
	{
		bool isbad = false;
		
		HObject Region;
		Threshold(m_Image, &Region, 128, 255);
		SetDraw(m_WindowHandle, "margin");
		SetColor(m_WindowHandle, "red");
		DispObj(Region, m_WindowHandle);

		/*if ()
		{*/
			isbad = true;
		//}
		

		emit resultReady(isbad);
	}
		
}