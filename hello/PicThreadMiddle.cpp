#include "PicThreadMiddle.h"
#include <QTime>
#include "CHH.h"

int PicThreadMiddle::num = 0;
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

			num++;
			CHH::disp_message(m_WindowHandle, HTuple("number: ") + num, "image", 12, 12, "red", "true");

			qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
			if (num % 3)
			{
				emit resultReady(MiddleGood);
				CHH::disp_message(m_WindowHandle, HTuple("Good "), "image", 120, 12, "red", "true");
			}
			else
			{
				emit resultReady(MiddleBad);
				CHH::disp_message(m_WindowHandle, HTuple("Bad "), "image", 120, 12, "red", "true");
			}
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