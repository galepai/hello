#include "PicThreadLeft.h"
#include <QTime>


void PicThreadLeft::run()
{
	//qDebug() << "Worker Run Thread : " << QThread::currentThreadId();

	if (m_Image.Key() != 0)
	{
		try
		{
			HObject Region;
			Threshold(m_Image, &Region, 5, 200);
			SetDraw(m_WindowHandle, "margin");
			SetColor(m_WindowHandle, "red");
			DispObj(Region, m_WindowHandle);

			qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
			if (qrand() % 3)
			{
				emit resultReady(LeftGood);
			}
			else
				emit resultReady(LeftBad);
		}
		catch (HException& e)
		{
			QString error = e.ErrorMessage().Text();
		}
	}
		
}