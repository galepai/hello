#include "PicThreadMiddle.h"
#include <QTime>


void PicThreadMiddle::run()
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

			OnHandle(m_WindowHandle);

			qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
			if (qrand() % 3)
			{
				emit resultReady(MiddleGood);
			}
			else
				emit resultReady(MiddleBad);
		}
		catch (HException& e)
		{
			QString error = e.ErrorMessage().Text();
		}
	}
		
}

void PicThreadMiddle::OnHandle(HTuple WindowHandle)
{
	static int step = 1;
	step++;

	HObject circle;
	int row = 100 + 50 * step;
	if (row<2000)
		GenCircle(&circle, 100 + 50 * step, 300, 100);
	else
	{
		GenCircle(&circle, 100 + 50 * step, 300, 100);
		step = 1;
	}

	DispObj(circle, WindowHandle);
}